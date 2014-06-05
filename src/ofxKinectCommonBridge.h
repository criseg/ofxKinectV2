#pragma once

#include "ofMain.h"

#include "KCBv2LIB.h"
#pragma comment (lib, "KCBv2.lib") // add path to lib additional dependency dir $(TargetDir)

typedef map<JointType, Kv2Joint> Skeleton;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// not sure this is right
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Kv2Joint
{

public:

	ofVec3f getPosition() {
		return ofJointPosition;
	}

	void setJoint(_Joint joint, _JointOrientation orientation)
	{
		ofJointRotation.set(orientation.Orientation.x, orientation.Orientation.y, orientation.Orientation.z, orientation.Orientation.w);
		ofJointPosition.set(joint.Position.X, joint.Position.Y, joint.Position.Z);
	}

	_Joint getJoint() {
		return kcbJoint;
	}

	_JointOrientation getOrientation() {
		return kcbOrientation;
	}

private:
	ofVec3f ofJointPosition;
	ofQuaternion ofJointRotation;
	_JointOrientation kcbOrientation;
	_Joint kcbJoint;
};

struct Kv2JointBackBuffer
{
	_JointOrientation kcbOrientation;
	_Joint kcbJoint;
	JointType type;
	TrackingState trackingState;
};

typedef vector<Kv2JointBackBuffer> skeletonBackBuffer;

class ofxKinectCommonBridge : protected ofThread {
  public:
	
	ofxKinectCommonBridge();

	// new API
	bool initSensor( int id = 0 );
	bool initDepthStream( bool mapDepthToColor = false );
	bool initColorStream( bool mapColorToDepth = false );
	bool initIRStream( int width, int height );
	bool initSkeletonStream( bool seated );
	bool start();

	void stop();

  	/// is the current frame new?
	bool isFrameNew();
	bool isFrameNewVideo();
	bool isFrameNewDepth();
	bool isNewSkeleton();
	bool initBodyIndexStream();

	void setDepthClipping(float nearClip=500, float farClip=4000);
	
	/// updates the pixel buffers and textures
	///
	/// make sure to call this to update to the latest incoming frames
	void update();
	ofPixels& getColorPixelsRef();
	ofPixels & getDepthPixelsRef();       	///< grayscale values
	ofShortPixels & getRawDepthPixelsRef();	///< raw 11 bit values

	/// enable/disable frame loading into textures on update()
	void setUseTexture(bool bUse);

	/// draw the video texture
	void draw(float x, float y, float w, float h);
	void draw(float x, float y);
	void draw(const ofPoint& point);
	void draw(const ofRectangle& rect);

	/// draw the grayscale depth texture
	void drawRawDepth(float x, float y, float w, float h);
	void drawRawDepth(float x, float y);
	void drawRawDepth(const ofPoint& point);
	void drawRawDepth(const ofRectangle& rect);

	/// draw the grayscale depth texture
	void drawDepth(float x, float y, float w, float h);
	void drawDepth(float x, float y);
	void drawDepth(const ofPoint& point);
	void drawDepth(const ofRectangle& rect);

	void drawIR( float x, float y, float w, float h );

	void drawBodyIndex(float x, float y);

	//vector<Skeleton> &getSkeletons();
	void drawSkeleton(int index);

	ofTexture &getRawDepthTexture() {
		return rawDepthTex;
	}

	ofTexture &getDepthTexture() {
		return depthTex;
	}


	ofTexture &getColorTexture() {
		return videoTex;
	}

  private:

    KCBHANDLE hKinect;
	//KINECT_IMAGE_FRAME_FORMAT depthFormat;
	ColorImageFormat colorFormat;
	//NUI_SKELETON_FRAME k4wSkeletons;

  	bool bInited;
	bool bStarted;
	vector<Skeleton> skeletons;

	//quantize depth buffer to 8 bit range
	vector<unsigned char> depthLookupTable;
	void updateDepthLookupTable();
	void updateDepthPixels();
	void updateIRPixels();
	bool bNearWhite;
	float nearClipping, farClipping;

  	bool bUseTexture;
	ofTexture depthTex; ///< the depth texture
	ofTexture rawDepthTex; ///<
	ofTexture videoTex; ///< the RGB texture
	ofTexture bodyIndexTex;
	//ofTexture irTex;

	ofPixels videoPixels;
	ofPixels videoPixelsBack;			///< rgb back
	ofPixels depthPixels;
	ofPixels depthPixelsBack;
	ofShortPixels depthPixelsRaw;
	ofShortPixels depthPixelsRawBack;	///< depth back

	ofShortPixels irPixelsRaw;
	ofShortPixels irPixelsBackRaw;
	ofPixels irPixels;
	ofPixels irPixelsBack;

	ofPixels bodyIndexPixelsBack;
	ofPixels bodyIndexPixels;

	skeletonBackBuffer skeletonBackBuffer[6];

	bool bIsFrameNewVideo;
	bool bNeedsUpdateVideo;
	bool bIsFrameNewDepth;
	bool bNeedsUpdateDepth;
	bool bNeedsUpdateSkeleton;
	bool bIsSkeletonFrameNew;
	bool bProgrammableRenderer;

	bool bNeedsUpdateBodyIndex;

	bool bVideoIsInfrared;
	bool bUsingSkeletons;
	bool bUsingDepth;
	bool bUsingBodyIndex;

	BYTE *irPixelByteArray;

	void threadedFunction();

	bool mappingColorToDepth;
	bool mappingDepthToColor;
	bool beginMappingColorToDepth;

	//NUI_IMAGE_RESOLUTION colorRes;
	//NUI_IMAGE_RESOLUTION depthRes;

	//INuiSensor *nuiSensor;
	//INuiCoordinateMapper *mapper;

	KCBDepthFrame* pDepthFrame;
	KCBColorFrame* pColorFrame;
	KCBInfraredFrame* pInfraredFrame;
	//KCBBodyFrame* pBodyFrame; // not using this yet
	//IBodyFrame *pbodyFrame;
	//IBody *pBodies[6];
	KCBBodyIndexFrame* pBodyIndexFrame;

	KCBFrameDescription colorFrameDescription;
	KCBFrameDescription depthFrameDescription;
	KCBFrameDescription irFrameDescription;
	KCBFrameDescription bodyIndexFrameDescription;
};
