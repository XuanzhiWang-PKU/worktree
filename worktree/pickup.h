#include <boost/shared_ptr.hpp>
#include <alcommon/almodule.h>
#include <alproxies/alvideodeviceproxy.h>
#include <alproxies/almotionproxy.h>
#include <alproxies/alrobotpostureproxy.h>

#include <opencv2/core/core.hpp>
#include <string>
using namespace cv;
namespace AL
{
class ALBroker;
}

class pickup : public AL::ALModule
{
  public:
	pickup(boost::shared_ptr<AL::ALBroker> pBroker, const std::string &pName);

	virtual ~pickup();

	void exit();

	virtual void init();

	void start();

	void stayPosture();

	void pickUpBalls();

	void throwOutBalls();

	void find(int num, Mat image);

	cv::Mat imgHeader;

  private:
	AL::ALVideoDeviceProxy camProxy;
	AL::ALMotionProxy motionProxy;
	AL::ALRobotPostureProxy postureProxy;
	std::string clientName;
};
