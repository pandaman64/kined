import std.stdio;

import kinect;

struct ColorImage{
	BGRA[] data;
	size_t rows_,cols_;
	
	this(uint r,uint c){
		rows_ = r;
		cols_ = c;
		data = new BGRA[](r * c);
	}
	this(this){
		data = data.dup;
	}
	
	auto ref opIndex(size_t idx) inout{
		return data[idx];
	}
	auto ref opIndex(size_t x,size_t y) inout{
		return data[y * cols + x];
	}
	
	@property{
		auto rows() const{
			return  rows_;
		}
		auto cols() const{
			return  cols_;
		}
		auto ref ptr() inout{
			return data.ptr;
		}
		auto length() const{
			return data.length;
		}
	}
}

void main(){
	InitializeKinect();
	scope(exit) FinalizeKinect();

	int width,height;
	GetImageSize(&width,&height);
	auto image = ColorImage(height,width);

	GetColorImage(image.ptr);
	
	"Done".writeln;
}
