#include <opencv2/core/core.hpp>
#include <opencv2/core/types_c.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <rtabmap/utilite/ULogger.h>
#include <rtabmap/utilite/UTimer.h>
#include <rtabmap/utilite/UConversion.h>
#include <rtabmap/utilite/UDirectory.h>
#include "rtabmap/core/Memory.h"
#include "rtabmap/core/util3d.h"

int main(int argc, char** argv)
{
	ULogger::setType(ULogger::kTypeConsole);
	ULogger::setLevel(ULogger::kInfo);

	std::string path = rtabmap::Parameters::defaultRtabmapWorkingDirectory() + "/rtabmap.db";
	if(argc > 1)
	{
		path = argv[1];
	}


    // Open database
	rtabmap::ParametersMap parameters;
	parameters.insert(rtabmap::ParametersPair(rtabmap::Parameters::kDbSqlite3InMemory(), "false"));
	rtabmap::Memory * memory = new rtabmap::Memory(parameters);
	if(!memory->init(path))
	{
		UWARN("Can't open database \"%s\"", path.c_str());
	}

	if(memory)
	{
		UINFO("Using database %s", path.c_str());
		std::string saveDirectory = "imagesExtracted/";
		if(!UDirectory::exists(saveDirectory))
		{
			UDirectory::makeDir(saveDirectory);
		}
		std::set<int> ids = memory->getAllSignatureIds();
		for(std::set<int>::iterator iter=ids.begin(); iter!=ids.end(); ++iter)
		{
			cv::Mat image = rtabmap::util3d::uncompressImage(memory->getImage(*iter));
			std::string fileName = uFormat("%d.png", *iter);
			cv::imwrite(saveDirectory+fileName, image);
			UINFO("Saved %s", (saveDirectory+fileName).c_str());
		}
	}

    return 0;
}