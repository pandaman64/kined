#pragma once

#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C"
#define EXTERN_C_END 
#elif
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif

EXTERN_C_BEGIN{
	//data of a pixel
	struct BGRA{
		unsigned char b,g,r,a;
	};

	//Wrapping Vector4
	struct Position{
		float x,y,z,w;
	};
	//Wrapping NUI_COLOR_IMAGE_POINT
	struct ColorPoint{
		long x,y;
	};
	//Wrapping NUI_DEPTH_IMAGE_POINT
	struct DepthPoint{
		float x,y;
	};

	//Wrapping NUI_SKELETON_TRACKING_STATE
	enum TrackingState{
		NOT_TRACKED,
		INFERRED,
		TRACKED
	};

	//extract important members from NUI_SKELETON_DATA
	struct SkeletonData{
		Position position;
		TrackingState state;
		Position skeleton_positions[20];
		TrackingState skeleton_states[20];
	};

	//Wrapping NUI_SKELETON_POSITION_INDEX
	enum SkeletonPositionIndex{
		HIP_CENTER,
		SPINE,
		SHOULDER_CENTER,
		HEAD,
		SHOULDER_LEFT,
		ELBOW_LEFT,
		WRIST_LEFT,
		HAND_LEFT,
		SHOULDER_RIGHT,
		ELBOW_RIGHT,
		WRIST_RIGHT,
		HAND_RIGHT,
		HIP_LEFT,
		KNEE_LEFT,
		ANKLE_LEFT,
		FOOT_LEFT,
		HIP_RIGHT,
		KNEE_RIGHT,
		ANKLE_RIGHT,
		FOOT_RIGHT,
		COUNT
    };

	//Initialize Kinect. If Kinect is already initialized, do nothing.
	void InitializeKinect();
	//Finalize Kinect. This function must be called at the end of the program.
	void FinalizeKinect();

	//Get the size of color/depth image.
	void GetImageSize(int *width,int *height);
	//Get color image. This function needs ptr can contain width*height elements.
	void GetColorImage(BGRA *ptr);
	//Get depth image. This function needs ptr can contain width*height elements.
	void GetDepthImage(unsigned short *ptr);
	//Get depth image. This function needs ptr can contain 6 elements.
	void GetSkeletons(SkeletonData *ptr);

	//functions as its name
	void TransformSkeletonPositionToDepthPoint(Position pos,DepthPoint *pt);
	void TransformDepthPointToColorPoint(DepthPoint dep,ColorPoint *col);
	void TransformSkeletonPositionToColorPoint(Position pos,ColorPoint *col);

	//helper
	inline unsigned short GetDepthNumber(unsigned short val){
		return val >> 3;
	}
	inline unsigned short GetPlayerIndex(unsigned short val){
		return val && 7;
	}
}
