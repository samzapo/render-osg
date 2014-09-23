// viewer.cpp
#include <osgDB/WriteFile>
#include <osg/CameraNode>
#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/PositionAttitudeTransform>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgGA/StandardManipulator>
#include <osgGA/StateSetManipulator>

class SnapImageDrawCallback : public ::osg::CameraNode::DrawCallback
{
       public:
       SnapImageDrawCallback()
       {
       _snapImageOnNextFrame = false;
       }

       void setFileName(const std::string& filename) { _filename = filename; }
       const std::string& getFileName() const { return _filename; }
       void setSnapImageOnNextFrame(bool flag) { _snapImageOnNextFrame = flag; }
       bool getSnapImageOnNextFrame() const { return _snapImageOnNextFrame; }
       virtual void operator () (const ::osg::CameraNode& camera) const
       {
         if (!_snapImageOnNextFrame) return;
//         ::osg::notify(::osg::NOTICE) << "Saving screen image to '"<<_filename<<"'"<< std::endl;
         int x,y,width,height;
         x = camera.getViewport()->x();
         y = camera.getViewport()->y();
         width = camera.getViewport()->width();
         height = camera.getViewport()->height();
//         ::osg::notify(::osg::NOTICE) << "Capturing image from: (" << x << ", " << y<< ")    " <<width<< " x "<< height << std::endl;

         ::osg::ref_ptr< ::osg::Image> image = new ::osg::Image();
         image->readPixels(x,y,width,height,GL_RGB,GL_UNSIGNED_BYTE);

         if (::osgDB::writeImageFile(*image,_filename)){
//           ::osg::notify(::osg::NOTICE) << "Saved screen image to '"<<_filename<<"'"<< std::endl;
         } else {
           ::osg::notify(::osg::NOTICE) << "Image not saved"<< std::endl;
         }
         _snapImageOnNextFrame = false;
       }

       protected:

       ::std::string _filename;
       mutable bool _snapImageOnNextFrame;

};

void
renderSceneToImage(::osg::Node* node, const ::std::string& sFileName_,double position[3],double target[3],double up[3])
{
  osg::Group* root = new osg::Group();

  // Declare transform, initialize with defaults.

  osg::PositionAttitudeTransform* nodeXform =
     new osg::PositionAttitudeTransform();

  // Use the 'addChild' method of the osg::Group class to
  // add the transform as a child of the root node and the
  // node node as a child of the transform.

  root->addChild(nodeXform);

  nodeXform->addChild(node);

  // Declare and initialize a Vec3 instance to change the
  // position of the node model in the scene
  osg::Vec3 nodePosit(5,0,0);
  nodeXform->setPosition( nodePosit );

  // Declare a 'viewer'
  osgViewer::Viewer viewer;

  // Next we will need to assign the scene graph we created
  // above to this viewer:
  viewer.setSceneData( root );

  viewer.setCameraManipulator(new osgGA::TrackballManipulator());
  viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));

  osg::Vec3d position_osg(position[0], position[1], position[2]);
  osg::Vec3d target_osg(target[0], target[1], target[2]);
  osg::Vec3d up_osg(up[0], up[1], up[2]);

  // set the camera view

  osg::Camera* camera = viewer.getCamera();
  camera->setViewMatrixAsLookAt(position_osg, target_osg, up_osg);

  // setup the manipulator using the camera, if necessary
  viewer.getCameraManipulator()->setHomePosition(position_osg, target_osg, up_osg);

//  camera->setRenderTargetImplementation(::osg::CameraNode::FRAME_BUFFER_OBJECT);

  ::osg::ref_ptr<SnapImageDrawCallback> snapImageDrawCallback = new SnapImageDrawCallback();
  camera->setPostDrawCallback (snapImageDrawCallback.get());

  snapImageDrawCallback->setFileName(sFileName_);
  snapImageDrawCallback->setSnapImageOnNextFrame(true);

  viewer.realize();

  int x,y,width,height;
  x = camera->getViewport()->x();
  y = camera->getViewport()->y();
  width = camera->getViewport()->width();
  height = camera->getViewport()->height();
//    ::osg::notify(::osg::NOTICE) << "Capturing image from: (" << x << ", " << y<< ")    " <<width<< " x "<< height << std::endl;

  // Prevent this from opening a window by making pbuffer context
//  osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
//  traits->x = 0;
//  traits->y = 0;
//  traits->width = width;
//  traits->height = height;
//  traits->red = 8;
//  traits->green = 8;
//  traits->blue = 8;
//  traits->alpha = 8;
//  traits->windowDecoration = false;
//  traits->pbuffer = true;
//  traits->doubleBuffer = true;
//  traits->sharedContext = 0;

//  osg::ref_ptr<osg::GraphicsContext> pbuffer;
//  pbuffer = ::osg::GraphicsContext::createGraphicsContext(traits.get());
//  if (pbuffer.valid())
//  {
//      ::osg::notify(osg::NOTICE)<<"Pixel buffer has been created successfully."<<std::endl;
//  }
//  else
//  {
//      ::osg::notify(osg::NOTICE)<<"Pixel buffer has not been created successfully."<<std::endl;
//  }

//  if (pbuffer.valid())
//  {
//      osg::ref_ptr<osg::Camera> camera = new osg::Camera;
//      camera->setGraphicsContext(pbuffer.get());
//      camera->setViewport(new osg::Viewport(0,0,width,height));
//      GLenum buffer = pbuffer->getTraits()->doubleBuffer ? GL_BACK : GL_FRONT;
//      camera->setDrawBuffer(buffer);
//      camera->setReadBuffer(buffer);
////      camera->setFinalDrawCallback(new WindowCaptureCallback(mode, position, readBuffer));
//      camera->setFinalDrawCallback(snapImageDrawCallback.get());

//      viewer.addSlave(camera.get(), osg::Matrixd(), osg::Matrixd());

      viewer.realize();

      viewer.frame();
//  }

}

void render(::osg::Node* node, const ::std::string& sFileName_){

  osg::Group* root = new osg::Group();

  // Declare transform, initialize with defaults.

  osg::PositionAttitudeTransform* nodeXform =
     new osg::PositionAttitudeTransform();

  // Use the 'addChild' method of the osg::Group class to
  // add the transform as a child of the root node and the
  // node node as a child of the transform.

  root->addChild(nodeXform);

  nodeXform->addChild(node);

  // Declare and initialize a Vec3 instance to change the
  // position of the node model in the scene
  osg::Vec3 nodePosit(5,0,0);
  nodeXform->setPosition( nodePosit );

  // Declare a 'viewer'
  osgViewer::Viewer viewer;

  // Next we will need to assign the scene graph we created
  // above to this viewer:
  viewer.setSceneData( root );

  // attach a trackball manipulator to all user control of the view
  viewer.setCameraManipulator(new osgGA::TrackballManipulator);

  // create the windows and start the required threads.
  viewer.realize();

  // Enter the simulation loop. viewer.done() returns false
  // until the user presses the 'esc' key.
  // (This can be changed by adding your own keyboard/mouse
  // event handler or by changing the settings of the default
  // keyboard/mouse event handler)

  while( !viewer.done() )
  {
     // dispatch the new frame, this wraps up the follow Viewer operations:
     //   advance() to the new frame
     //   eventTraversal() that collects events and passes them on to the event handlers and event callbacks
     //   updateTraversal() to calls the update callbacks
     //   renderingTraversals() that runs syncronizes all the rendering threads (if any) and dispatch cull, draw and swap buffers
     viewer.frame();
  }
}

int main(int argc, char** argv)
{
  ::osg::Node* pRoot;
  std::string sFileName;

  // get all options
  pRoot = osgDB::readNodeFile(argv[1]);
  sFileName = std::string(argv[argc-1]);
  double position[3] = {1,1,1};
  double target[3] = {0,0,0};
  double up[3] = {0,0,1};

  for (int i=2; i< argc-1; i++)
  {
    // get the option
    std::string option(argv[i]);

    if (option.find("-p") != std::string::npos){
      position[0] = std::atof(&argv[i+1][0]) + 5.0;
      position[1] = std::atof(&argv[i+2][0]);
      position[2] = std::atof(&argv[i+3][0]);
    } else if (option.find("-t") != std::string::npos){
      target[0] = std::atof(&argv[i+1][0]) + 5.0;
      target[1] = std::atof(&argv[i+2][0]);
      target[2] = std::atof(&argv[i+3][0]);
    }
  }
//  ::osg::notify(::osg::NOTICE) << "Capturing image from: (" << position[0]-5.0 << ", " << position[1]<<", " << position[2]<< ")  "
//                                  "of object at  (" << target[0]-5.0 << ", " << target[1]<<", " << target[2]<< ")" << std::endl;

  renderSceneToImage(pRoot,sFileName,position,target,up);
//  render(pRoot,sFileName);
}
