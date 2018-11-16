#include <signal.h>
#include <boost/shared_ptr.hpp>
#include <alcommon/albroker.h>
#include <alcommon/almodule.h>
#include <alcommon/albrokermanager.h>
#include <alcommon/altoolsmain.h>

#include "pickup.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
  int pport = 9559;
  std::string pip = "10.0.0.3";
  if (argc != 1 && argc != 3 && argc != 5)
  {
    std::cerr << "Wrong number of arguments!" << std::endl;
    std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
    exit(2);
  }
  if (argc == 3)
  {
    if (std::string(argv[1]) == "--pip")
      pip = argv[2];
    else if (std::string(argv[1]) == "--pport")
      pport = atoi(argv[2]);
    else
    {
      std::cerr << "Wrong number of arguments!" << std::endl;
      std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
      exit(2);
    }
  }

  if (argc == 5)
  {
    if (std::string(argv[1]) == "--pport"
        && std::string(argv[3]) == "--pip")
    {
      pport = atoi(argv[2]);
      pip = argv[4];
    }
    else if (std::string(argv[3]) == "--pport"
             && std::string(argv[1]) == "--pip")
    {
      pport = atoi(argv[4]);
      pip = argv[2];
    }
    else
    {
      std::cerr << "Wrong number of arguments!" << std::endl;
      std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
      exit(2);
    }
  }
  const std::string brokerName = "pickup";

  int brokerPort = 54000;
  
  const std::string brokerIp = "0.0.0.0";

  boost::shared_ptr<AL::ALBroker> broker;

  try
  {
    broker = AL::ALBroker::createBroker(
        brokerName,
        brokerIp,
        brokerPort,
        pip,
        pport,
        0    // you can pass various options for the broker creation,
             // but default is fine
      );
  }
  catch(const AL::ALError &e)
  {
    std::cerr << "Fail to connect broker to: "
              << pip
              << ":"
              << pport
              << std::endl;

    AL::ALBrokerManager::getInstance()->killAllBroker();
    AL::ALBrokerManager::kill();
    return 1;
  }

  AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(broker);
  AL::ALModule::createModule<pickup>(broker,"pickup" );
  return 0;
}
