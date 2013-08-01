#include "Configuration.h"

#include <iostream>
#include <stdio.h>
#include <yaml-cpp/yaml.h>

Configuration::Configuration()
{
	WindowWidth = 800;
	WindowHeight = 600;
	WindowTitle = "Simulation";
}

Configuration::Configuration(const char* filename)
{
	printf("Loading configuration data from: %s\n", filename);
	YAML::Node config = YAML::LoadFile(filename);

	WindowWidth = config["width"].as<int>();
	WindowHeight = config["height"].as<int>();
	WindowTitle = config["title"].as<string>();

	printf("Configuration Loaded!\n");
	printf("\tWidth: %d\n", WindowWidth);
	printf("\tHeight: %d\n", WindowHeight);
	printf("\tTitle: %s\n", WindowTitle.c_str());
}

Configuration::~Configuration(){}
