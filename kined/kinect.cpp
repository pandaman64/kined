#include "Windows.h"
#include "NuiApi.h"

#include <iostream>
#include <memory>

#include "kinect.h"

#define ERROR_CHECK(x) \
	if(x == S_OK){ \
	} \
	else{ \
		std::cerr << "In file:" << __FILE__ << " line:" << __LINE__ << '\n' << #x << '\n' << " :(" << std::endl; \
		std::terminate(); \
	}

EXTERN_C_BEGIN{
	static INuiSensor *kinect_sensor;
	static NUI_IMAGE_RESOLUTION resolution = NUI_IMAGE_RESOLUTION_640x480;
	static HANDLE color_stream,depth_stream;
	static DWORD width,height;

	void InitializeKinect(){
		if(kinect_sensor){
			return;
		}
		ERROR_CHECK(NuiCreateSensorByIndex(0,&kinect_sensor));
		ERROR_CHECK(kinect_sensor->NuiInitialize(NUI_INITIALIZE_FLAG_USES_COLOR | NUI_INITIALIZE_FLAG_USES_DEPTH_AND_PLAYER_INDEX | NUI_INITIALIZE_FLAG_USES_SKELETON));
		ERROR_CHECK(kinect_sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_COLOR,resolution,0,2,0,&color_stream));
		ERROR_CHECK(kinect_sensor->NuiImageStreamOpen(NUI_IMAGE_TYPE_DEPTH_AND_PLAYER_INDEX,resolution,0,2,0,&depth_stream));
		ERROR_CHECK(kinect_sensor->NuiSkeletonTrackingEnable(0,0));
		NuiImageResolutionToSize(resolution,width,height);
	}
	void FinalizeKinect(){
		kinect_sensor->NuiShutdown();
		kinect_sensor->Release();
	}

	void GetImageSize(int *wid,int *hei){
		*wid = width;
		*hei = height;
	}
	void GetColorImage(BGRA *ptr){
		NUI_IMAGE_FRAME image_frame = {0};
		ERROR_CHECK(kinect_sensor->NuiImageStreamGetNextFrame(color_stream,INFINITE,&image_frame));

		NUI_LOCKED_RECT color_data;
		ERROR_CHECK(image_frame.pFrameTexture->LockRect(0,&color_data,0,0));
		memcpy(ptr,color_data.pBits,sizeof(BGRA) * 640 * 480);

		ERROR_CHECK(kinect_sensor->NuiImageStreamReleaseFrame(color_stream,&image_frame));
	}
	void GetDepthImage(unsigned short *ptr){
		NUI_IMAGE_FRAME depth_frame = {};
		ERROR_CHECK(kinect_sensor->NuiImageStreamGetNextFrame(depth_stream,INFINITE,&depth_frame));

		NUI_LOCKED_RECT depth_data;
		depth_frame.pFrameTexture->LockRect(0,&depth_data,0,0);
		memcpy(ptr,depth_data.pBits,sizeof(USHORT) * 640 * 480);

		ERROR_CHECK(kinect_sensor->NuiImageStreamReleaseFrame(depth_stream,&depth_frame));
	}
	void GetSkeletons(SkeletonData *ptr){
		NUI_SKELETON_FRAME skeleton_frame = {};
		ERROR_CHECK(kinect_sensor->NuiSkeletonGetNextFrame(INFINITE,&skeleton_frame));

		for(int i = 0;i < NUI_SKELETON_COUNT;i++){
			auto const& skeleton = skeleton_frame.SkeletonData[i];
			auto &data = ptr[i];

			memcpy(&data.position,&skeleton.Position,sizeof(Vector4));
			memcpy(data.skeleton_positions,&skeleton.SkeletonPositions,sizeof(Vector4) * 20);
			memcpy(&data.state,&skeleton.eTrackingState,sizeof(NUI_SKELETON_TRACKING_STATE));
			memcpy(&data.skeleton_states,&skeleton.eSkeletonPositionTrackingState,sizeof(NUI_SKELETON_TRACKING_STATE) * 20);
		}
	}

	void TransformSkeletonPositionToDepthPoint(Position pos,DepthPoint *pt){
		NuiTransformSkeletonToDepthImage(reinterpret_cast<Vector4&>(pos),&pt->x,&pt->y,resolution);
	}
	void TransformDepthPointToColorPoint(DepthPoint dep,ColorPoint *col){
		ERROR_CHECK(kinect_sensor->NuiImageGetColorPixelCoordinatesFromDepthPixelAtResolution(
			resolution,resolution,0,dep.x,dep.y,0,&col->x,&col->y
		));
	}
	void TransformSkeletonPositionToColorPoint(Position pos,ColorPoint *col){
		DepthPoint dep;
		TransformSkeletonPositionToDepthPoint(pos,&dep);
		TransformDepthPointToColorPoint(dep,col);
	}
}
