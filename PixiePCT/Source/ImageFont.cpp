//*** ImageFont.cpp **

#include "ImageFont.h"
#include "Image.h"
#include "StandardLibrary.h"
#include "Bitmap_RLE8.h"
#include "HashTable.h"
#include "ColorHelper.h"
#include "Asset.h"
#include "Bitmap.h"


//*** Constructor ***

ImageFont::ImageFont():
	size_(0),
	glyphHeight_(0)
	{
	}


//*** Destructor ***

ImageFont::~ImageFont()
	{
	for (int i=0; i<glyphs_.GetItemCount(); i++)
		{
		Glyph* glyph=glyphs_.Get(i);
		for (int j=0; j<glyph->imageLayers.GetItemCount(); j++)
			{
			delete glyph->imageLayers.Get(j);
			}
		for (int j=0; j<glyph->bitmapLayers.GetItemCount(); j++)
			{
			delete glyph->bitmapLayers.Get(j);
			}
		delete glyph;
		}
	}


//*** LoadFromXML ***

void ImageFont::LoadFromXML(const char* filename)
	{
	// Load font definition
	LoadXML(filename);

	// Find path
	const char* l1=StrRChr(filename,'/');
	const char* l2=StrRChr(filename,'\\');
	const char* l=l1;
	if (l2>l)
		{
		l=l2;
		}
	char path[1024];
	if (l>0)
		{
		int len=(int)(l-filename+1);
		StrNCpy(path,filename,len);
		path[len]=0;
		}
	else
		{
		StrCpy(path,"");
		}
    
	layerCount_=layerNames_.GetItemCount();

	// Create images
	for (int i=0; i<layerNames_.GetItemCount(); i++)
		{
		const char* filename=layerNames_.Get(i);
		char filenameFullPath[1024];
		SNPrintF(filenameFullPath,1024,"%s%s",path,filename);
		Image layer(filenameFullPath);
		
		for (int i=0; i<glyphs_.GetItemCount(); i++)
			{
			Glyph* glyph=glyphs_.Get(i);
			int hpitch=glyph->x2-glyph->x1+1;
			int vpitch=glyph->y2-glyph->y1+1;
			Image* image=new Image(hpitch,vpitch);
			for (int y=0; y<vpitch; y++)
				{
				for (int x=0; x<hpitch; x++)
					{
					image->SetPixel(x,y,layer.GetPixel(glyph->x1+x,glyph->y1+y));
					}
				}

			glyph->imageLayers.Add(image);
			}
		}

	// Delete layer names
	for (int i=0; i<layerNames_.GetItemCount(); i++)
		{
		char* filename=layerNames_.Get(i);
		delete filename;
		}
	layerNames_.Clear(true);
	}


//*** GetGlyphCount ***

int ImageFont::GetGlyphCount()
	{
	return glyphs_.GetItemCount();
	}


//*** GetGlyph ***

ImageFont::Glyph* ImageFont::GetGlyph(int index)
	{
	return glyphs_.Get(index);
	}

		
//*** XML_Element ***

XMLObject* ImageFont::XML_Element(StringId _name, const XMLAttributeList& _attributes) 
	{
	xmlCase(Glyph)
		{
		Glyph* glyph=new Glyph;
		glyph->ascii=xmlAttributeValue("ascii");
		glyph->width=xmlAttributeValue("width");
		glyph->x1=xmlAttributeValue("x1");
		glyph->y1=xmlAttributeValue("y1");
		glyph->x2=xmlAttributeValue("x2");
		glyph->y2=xmlAttributeValue("y2");
		glyph->xoffset=xmlAttributeValue("xoffset");
		glyph->yoffset=xmlAttributeValue("yoffset");
		glyphs_.Add(glyph);
		}

	xmlCase(Kerning)
		{
		Kerning kerning;
		kerning.first=xmlAttributeValue("first");
		kerning.second=xmlAttributeValue("second");
		kerning.amount=xmlAttributeValue("amount");
		kerning_.Add(kerning);
		}

	xmlCase(Layer)
		{
		char* filename=StrDup(xmlAttributeValue("filename"));
		layerNames_.Add(filename);		
		}

	return 0;
	}


//*** XML_Attribute ***

void ImageFont::XML_Attribute(StringId _name, const XMLVariant& _value) 
	{
	xmlCase(name)
		{
		name_=StringId(xmlValue);
		}

	xmlCase(size)
		{
		size_=xmlValue;
		}

	xmlCase(glyphHeight)
		{
		glyphHeight_=xmlValue;
		}

	xmlCase(baseline)
		{
		baseline_=xmlValue;
		}
	}


//*** WriteToAsset ***

void ImageFont::WriteToAsset(Asset* asset)
	{
	int nameLength=StrLen(name_.GetString());
	asset->Write(&nameLength);
	asset->Write(name_.GetString(),nameLength);
	asset->Write(&size_);
	asset->Write(&glyphHeight_);
	asset->Write(&baseline_);
	asset->Write(&layerCount_);
	int glyphCount=glyphs_.GetItemCount();
	asset->Write(&glyphCount);

	for (int i=0; i<glyphs_.GetItemCount(); i++)
		{
		Glyph* glyph=glyphs_.Get(i);
		asset->Write(&glyph->ascii);
		asset->Write(&glyph->width);
		asset->Write(&glyph->xoffset);
		asset->Write(&glyph->yoffset);
		for (int i=0; i<layerCount_; i++)
			{
			glyph->bitmapLayers.Get(i)->WriteToAsset(asset);
			}
		}

	int kerningCount=kerning_.GetItemCount();
	asset->Write(&kerningCount);

	for (int i=0; i<kerning_.GetItemCount(); i++)
		{
		Kerning kerning=kerning_.Get(i);
		asset->Write(&kerning.first);
		asset->Write(&kerning.second);
		asset->Write(&kerning.amount);
		}
	}

