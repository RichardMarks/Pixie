//*** MedianCutPalettizer.h ***

#include "MedianCutPalettizer.h"
#include "ColorHelper.h"


#pragma warning (disable:4244)
#pragma warning (disable:4530)
#include <list>

#include <limits>

#include <queue>
#include <algorithm>


const int NUM_DIMENSIONS = 3;

struct Point
{
    unsigned char x[NUM_DIMENSIONS];
};

class Block
{
    Point minCorner, maxCorner;
    Point* points;
    int pointsLength;
public:
    Block(Point* points, int pointsLength);
    Point * getPoints();
    int numPoints() const;
    int longestSideIndex() const;
    int longestSideLength() const;
    bool operator<(const Block& rhs) const;
    void shrink();
private:
    template <typename T>
    static T min(const T a, const T b)
    {
        if (a < b)
            return a;
        else
            return b;
    }

    template <typename T>
    static T max(const T a, const T b)
    {
        if (a > b)
            return a;
        else
            return b;
    }

};

template <int index>
class CoordinatePointComparator
{
public:
    bool operator()(Point left, Point right)
    {
        return left.x[index] < right.x[index];
    }
};


Block::Block(Point* points, int pointsLength)
{
    this->points = points;
    this->pointsLength = pointsLength;
    for(int i=0; i < NUM_DIMENSIONS; i++)
    {
        minCorner.x[i] = std::numeric_limits<unsigned char>::min();
        maxCorner.x[i] = std::numeric_limits<unsigned char>::max();
    }
}

Point * Block::getPoints()
{
    return points;
}

int Block::numPoints() const
{
    return pointsLength;
}

int Block::longestSideIndex() const
{
    int m = maxCorner.x[0] - minCorner.x[0];
    int maxIndex = 0;
    for(int i=1; i < NUM_DIMENSIONS; i++)
    {
        int diff = maxCorner.x[i] - minCorner.x[i];
        if (diff > m)
        {
            m = diff;
            maxIndex = i;
        }
    }
    return maxIndex;
}

int Block::longestSideLength() const
{
    int i = longestSideIndex();
    return maxCorner.x[i] - minCorner.x[i];
}

bool Block::operator<(const Block& rhs) const
{
    return this->longestSideLength() < rhs.longestSideLength();
}

void Block::shrink()
{
    int i,j;
    for(j=0; j<NUM_DIMENSIONS; j++)
    {
        minCorner.x[j] = maxCorner.x[j] = points[0].x[j];
    }
    for(i=1; i < pointsLength; i++)
    {
        for(j=0; j<NUM_DIMENSIONS; j++)
        {
            minCorner.x[j] = min(minCorner.x[j], points[i].x[j]);
            maxCorner.x[j] = max(maxCorner.x[j], points[i].x[j]);
        }
    }
}

std::list<Point> medianCut(Point* image, int numPoints, unsigned int desiredSize)
{
    std::priority_queue<Block> blockQueue;

    Block initialBlock(image, numPoints);
    initialBlock.shrink();

    blockQueue.push(initialBlock);
    while (blockQueue.size() < desiredSize && blockQueue.top().numPoints() > 1)
    {
        Block longestBlock = blockQueue.top();

        blockQueue.pop();
        Point * begin  = longestBlock.getPoints();
	Point * median = longestBlock.getPoints() + (longestBlock.numPoints()+1)/2;
	Point * end    = longestBlock.getPoints() + longestBlock.numPoints();
	switch(longestBlock.longestSideIndex())
	{
	case 0: std::nth_element(begin, median, end, CoordinatePointComparator<0>()); break;
	case 1: std::nth_element(begin, median, end, CoordinatePointComparator<1>()); break;
	case 2: std::nth_element(begin, median, end, CoordinatePointComparator<2>()); break;
	}

	Block block1(begin, median-begin), block2(median, end-median);
	block1.shrink();
	block2.shrink();

        blockQueue.push(block1);
        blockQueue.push(block2);
    }

    std::list<Point> result;
    while(!blockQueue.empty())
    {
        Block block = blockQueue.top();
        blockQueue.pop();
        Point * points = block.getPoints();

        int sum[NUM_DIMENSIONS] = {0};
        for(int i=0; i < block.numPoints(); i++)
        {
            for(int j=0; j < NUM_DIMENSIONS; j++)
            {
                sum[j] += points[i].x[j];
            }
        }

        Point averagePoint;
        for(int j=0; j < NUM_DIMENSIONS; j++)
        {
            averagePoint.x[j] = sum[j] / block.numPoints();
        }

        result.push_back(averagePoint);
    }

    return result;
}



unsigned char FindNearestColor(unsigned int color, unsigned int* palette, int paletteCount) 
	{
    int i, distanceSquared, minDistanceSquared, bestIndex = 0;
    minDistanceSquared = 255*255 + 255*255 + 255*255 + 1;
    for (i=0; i<paletteCount; i++) 
		{
		unsigned char cR=((unsigned char)((color&0x00ff0000)>>16));
		unsigned char cG=((unsigned char)((color&0x0000ff00)>>8 ));
		unsigned char cB=((unsigned char)((color&0x000000ff)    ));
		unsigned char pR=((unsigned char)((palette[i]&0x00ff0000)>>16));
		unsigned char pG=((unsigned char)((palette[i]&0x0000ff00)>>8 ));
		unsigned char pB=((unsigned char)((palette[i]&0x000000ff)    ));
        int Rdiff = ((int)cR) - pR;
        int Gdiff = ((int)cG) - pG;
        int Bdiff = ((int)cB) - pB;
        distanceSquared = Rdiff*Rdiff + Gdiff*Gdiff + Bdiff*Bdiff;
        if (distanceSquared < minDistanceSquared) 
			{
            minDistanceSquared = distanceSquared;
            bestIndex = i;
			}
		}
    return (unsigned char)bestIndex;
	}


unsigned char FindNearestColor(unsigned short color, unsigned short* palette, int paletteCount) 
	{
    int i, distanceSquared, minDistanceSquared, bestIndex = 0;
    minDistanceSquared = 255*255 + 255*255 + 255*255 + 1;
    for (i=0; i<paletteCount; i++) 
		{
		unsigned char cR=((unsigned char)((RGB16TO32(color)&0x00ff0000)>>16));
		unsigned char cG=((unsigned char)((RGB16TO32(color)&0x0000ff00)>>8 ));
		unsigned char cB=((unsigned char)((RGB16TO32(color)&0x000000ff)    ));
		unsigned char pR=((unsigned char)((RGB16TO32(palette[i])&0x00ff0000)>>16));
		unsigned char pG=((unsigned char)((RGB16TO32(palette[i])&0x0000ff00)>>8 ));
		unsigned char pB=((unsigned char)((RGB16TO32(palette[i])&0x000000ff)    ));
        int Rdiff = ((int)cR) - pR;
        int Gdiff = ((int)cG) - pG;
        int Bdiff = ((int)cB) - pB;
        distanceSquared = Rdiff*Rdiff + Gdiff*Gdiff + Bdiff*Bdiff;
        if (distanceSquared < minDistanceSquared) 
			{
            minDistanceSquared = distanceSquared;
            bestIndex = i;
			}
		}
    return (unsigned char)bestIndex;
	}


//*** GeneratePalette ***

int MedianCutPalettizer::GeneratePalette(unsigned short* imageData, int imageWidth, int imageHeight, unsigned short* palette, int paletteMaxCount)
	{
	int colorCount=imageWidth*imageHeight;
	Point* data=new Point[colorCount];
	int dataSize=0;
	for (int p=0; p<colorCount; p++)
		{
		unsigned int color=RGB16TO32(*imageData);
		imageData++;
		unsigned char a=((unsigned char)((color&0xff000000)>>24));
		if (a>0)
			{
			unsigned char r=((unsigned char)((color&0x00ff0000)>>16));
			unsigned char g=((unsigned char)((color&0x0000ff00)>>8 ));
			unsigned char b=((unsigned char)((color&0x000000ff)    ));
			data[dataSize].x[0]=r;
			data[dataSize].x[1]=g;
			data[dataSize].x[2]=b;
			dataSize++;
			}
		}
	std::list<Point> result=medianCut(data,dataSize,paletteMaxCount);
	delete[] data;
	
	std::list<Point>::iterator it=result.begin();
	int i=0;
	while (it!=result.end() && i<paletteMaxCount)
		{
		Point p=*it;
		unsigned int c=0xff000000;
		c|=p.x[0]<<16;
		c|=p.x[1]<<8;
		c|=p.x[2];
		int found=false;
		for (int j=0; j<i; j++)
			{
			if (palette[j]==RGB32TO16(c))
				{
				found=true;
				break;
				}
			}
		if (!found)
			{
			palette[i]=RGB32TO16(c);
			i++;
			}
		it++;
		}

	return i;
	}


//*** GeneratePalette ***

int MedianCutPalettizer::GeneratePalette(unsigned int* imageData, int imageWidth, int imageHeight, unsigned int* palette, int paletteMaxCount)
	{
	int colorCount=imageWidth*imageHeight;
	Point* data=new Point[colorCount];
	int dataSize=0;
	for (int p=0; p<colorCount; p++)
		{
		unsigned int color=*imageData;
		imageData++;
		unsigned char a=((unsigned char)((color&0xff000000)>>24));
		if (a>0)
			{
			unsigned char r=((unsigned char)((color&0x00ff0000)>>16));
			unsigned char g=((unsigned char)((color&0x0000ff00)>>8 ));
			unsigned char b=((unsigned char)((color&0x000000ff)    ));
			data[dataSize].x[0]=r;
			data[dataSize].x[1]=g;
			data[dataSize].x[2]=b;
			dataSize++;
			}
		}
	std::list<Point> result=medianCut(data,dataSize,paletteMaxCount);
	delete[] data;
	
	std::list<Point>::iterator it=result.begin();
	int i=0;
	while (it!=result.end() && i<paletteMaxCount)
		{
		Point p=*it;
		unsigned int c=0xff000000;
		c|=p.x[0]<<16;
		c|=p.x[1]<<8;
		c|=p.x[2];
		int found=false;
		for (int j=0; j<i; j++)
			{
			if (palette[j]==c)
				{
				found=true;
				break;
				}
			}
		if (!found)
			{
			palette[i]=c;
			i++;
			}
		it++;
		}

	return i;
	}


//*** GeneratePalette ***

int MedianCutPalettizer::GeneratePalette(unsigned int* imageData, int imageWidth, int imageHeight, unsigned short* palette, int paletteMaxCount)
	{
	int colorCount=imageWidth*imageHeight;
	Point* data=new Point[colorCount];
	int dataSize=0;
	for (int p=0; p<colorCount; p++)
		{
		unsigned int color=*imageData;
		imageData++;
		unsigned char a=((unsigned char)((color&0xff000000)>>24));
		if (a>0)
			{
			color=RGB16TO32(RGB32TO16(color));
			unsigned char r=((unsigned char)((color&0x00ff0000)>>16));
			unsigned char g=((unsigned char)((color&0x0000ff00)>>8 ));
			unsigned char b=((unsigned char)((color&0x000000ff)    ));
			data[dataSize].x[0]=r;
			data[dataSize].x[1]=g;
			data[dataSize].x[2]=b;
			dataSize++;
			}
		}
	std::list<Point> result=medianCut(data,dataSize,paletteMaxCount);
	delete[] data;
	
	std::list<Point>::iterator it=result.begin();
	int i=0;
	while (it!=result.end() && i<paletteMaxCount)
		{
		Point p=*it;
		unsigned int c=0xff000000;
		c|=p.x[0]<<16;
		c|=p.x[1]<<8;
		c|=p.x[2];
		unsigned short c16=RGB32TO16(c);
		int found=false;
		for (int j=0; j<i; j++)
			{
			if (palette[j]==c16)
				{
				found=true;
				break;
				}
			}
		if (!found)
			{
			palette[i]=c16;
			i++;
			}
		it++;
		}

	return i;
	}


//*** PalettizeImage ***

void MedianCutPalettizer::PalettizeImage(unsigned int* imageData, int imageWidth, int imageHeight, unsigned int* palette, int paletteCount, unsigned char* outputData)
	{
	for (int y=0; y<imageHeight; y++)
		{
		for (int x=0; x<imageWidth; x++)
			{
			unsigned char paletteIndex=FindNearestColor(*imageData,palette,paletteCount);
			*outputData=paletteIndex;
			imageData++;
			outputData++;
			}
		}
	}


//*** PalettizeImage ***

void MedianCutPalettizer::PalettizeImage(unsigned short* imageData, int imageWidth, int imageHeight, unsigned short* palette, int paletteCount, unsigned char* outputData)
	{
	for (int y=0; y<imageHeight; y++)
		{
		for (int x=0; x<imageWidth; x++)
			{
			unsigned char paletteIndex=FindNearestColor(*imageData,palette,paletteCount);
			*outputData=paletteIndex;
			imageData++;
			outputData++;
			}
		}
	}
