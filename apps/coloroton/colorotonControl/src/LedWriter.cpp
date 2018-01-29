#include "LedWriter.h"



LedWriter::LedWriter()
{
	
}

LedWriter::~LedWriter()
{
}


void LedWriter::setup(VisSettings * settings_)
{
	settings = settings_;
}

void LedWriter::loadMapping(string mappingFile)
{
	vector<bool> isMaster;
	vector<int> nLeds;

	// read file
	ofBuffer buffer = ofBufferFromFile(mappingFile);

	if (buffer.size()) {
		for (auto& line : buffer.getLines()) {
			string code = ofSplitString(line, "#")[0];
			if (code[1] == 'S') isMaster.push_back(false);
			else isMaster.push_back(true);
			nLeds.push_back(ofToInt(code.substr(2)));
		}
	}

	//init mapping
	mapping.clear();
	settings->width = 0;
	nChannels = nLeds.size() / 2;

	for (auto& v : nLeds)
	{
		settings->width += v;
	}

	for (int i = 0; i < settings->width; i++)
	{
		ledIndex v;
		v.channel = -1;
		v.index = -1;
		vector<ledIndex> row = { v,v };
		mapping.push_back(row);
	}

	//create mapping
	int offset = 0;
	int channel = 0;
	for (int i = 0; i < nLeds.size(); i += 2)
	{
		if (nLeds[i] + nLeds[i + 1] > nLedsPerChannel) nLedsPerChannel = nLeds[i] + nLeds[i + 1];
		if (isMaster[i]) //left master
		{
			int index = 0;
			//left bottom
			for (int x = 0; x < nLeds[i]; x++)
			{
				mapping[offset + nLeds[i] - x - 1][1].channel = channel;
				mapping[offset + nLeds[i] - x - 1][1].index = index;
				++index;
			}
			//top
			for (int x = 0; x < nLeds[i + 1] + nLeds[i]; x++)
			{
				mapping[offset + x][0].channel = channel;
				mapping[offset + x][0].index = index;
				++index;
			}

			//rightbottom
			for (int x = 0; x < nLeds[i + 1]; x++)
			{
				mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].channel = channel;
				mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].index = index;
				++index;
			}
		} else //right master
		{
			//rightup
			int index = 0;
			for (int x = 0; x < nLeds[i + 1]; x++)
			{
				mapping[x + offset + nLeds[i]][0].channel = channel;
				mapping[x + offset + nLeds[i]][0].index = index;
				++index;
			}
			//bottom
			for (int x = 0; x < nLeds[i + 1] + nLeds[i]; x++)
			{
				mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].channel = channel;
				mapping[offset + nLeds[i] + nLeds[i + 1] - x - 1][1].index = index;
				++index;
			}
			//left top
			for (int x = 0; x < nLeds[i]; x++)
			{
				mapping[x + offset][0].channel = channel;
				mapping[x + offset][0].index = index;
				++index;
			}
		}

		//show mapping
		cout << "channel " << channel << endl << endl;

		for (int j = offset; j < nLeds[i] + nLeds[i + 1] + offset; j++)
		{
			cout << mapping[j][0].index << " ";
		}
		cout << endl;
		for (int j = offset; j < nLeds[i] + nLeds[i + 1] + offset; j++)
		{
			cout << mapping[j][1].index << " ";
		}
		cout << endl << endl << endl;

		//set offset
		offset += nLeds[i] + nLeds[i + 1];
		channel++;
	}

}
