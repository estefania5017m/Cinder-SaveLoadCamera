#include "SaveLoadCamera.h"
#include "cinder/Json.h"

namespace reza {
namespace cam {

void saveCamera( const ci::fs::path &path, const ci::CameraPersp &cam )
{
	float fov = cam.getFov();
	float aspect = cam.getAspectRatio();
	float near = cam.getNearClip();
	float far = cam.getFarClip();
	ci::vec3 eyePt = cam.getEyePoint();
	ci::vec3 ctrPt = cam.getPivotPoint();
	ci::vec3 wldUp = cam.getWorldUp();

	ci::JsonTree tree;
	tree.addChild( ci::JsonTree( "FOV", fov ) );
	tree.addChild( ci::JsonTree( "ASPECT", aspect ) );
	tree.addChild( ci::JsonTree( "NEAR", near ) );
	tree.addChild( ci::JsonTree( "FAR", far ) );
	tree.addChild( ci::JsonTree( "EX", eyePt.x ) );
	tree.addChild( ci::JsonTree( "EY", eyePt.y ) );
	tree.addChild( ci::JsonTree( "EZ", eyePt.z ) );
	tree.addChild( ci::JsonTree( "CX", ctrPt.x ) );
	tree.addChild( ci::JsonTree( "CY", ctrPt.y ) );
	tree.addChild( ci::JsonTree( "CZ", ctrPt.z ) );
	tree.addChild( ci::JsonTree( "WX", wldUp.x ) );
	tree.addChild( ci::JsonTree( "WY", wldUp.y ) );
	tree.addChild( ci::JsonTree( "WZ", wldUp.z ) );
	tree.write( path );
}

void loadCamera( const ci::fs::path &path, ci::CameraPersp &cam, const std::function<void()> &cb )
{
	if( ci::fs::exists( path ) ) {
		try {
			ci::JsonTree tree( ci::loadFile( path ) );
			if( tree.hasChildren() ) {
				try {
					ci::vec3 eyePt( tree.getValueForKey<float>( "EX" ), tree.getValueForKey<float>( "EY" ), tree.getValueForKey<float>( "EZ" ) );
                    cam.setEyePoint( eyePt );
					
                    ci::vec3 ctrPt( tree.getValueForKey<float>( "CX" ), tree.getValueForKey<float>( "CY" ), tree.getValueForKey<float>( "CZ" ) );
					cam.lookAt( ctrPt );
                    
                    ci::vec3 wldUp( tree.getValueForKey<float>( "WX" ), tree.getValueForKey<float>( "WY" ), tree.getValueForKey<float>( "WZ" ) );
                    cam.setWorldUp( wldUp );
                    
					float fov = tree.getValueForKey<float>( "FOV" );
					float near = tree.getValueForKey<float>( "NEAR" );
					float far = tree.getValueForKey<float>( "FAR" );
					float aspect = tree.getValueForKey<float>( "ASPECT" );
					cam.setPerspective( fov, aspect, near, far );

					if( cb != nullptr ) {
						cb();
					}
				}
				catch( ci::Exception exc ) {
					std::cout << "LOAD CAMERA ERROR: " << exc.what() << std::endl;
				}
			}
		}
		catch( ci::Exception exc ) {
			std::cout << "LOAD CAMERA ERROR: " << exc.what() << std::endl;
		}
	}
}
} // namespace cam
} // namespace reza
