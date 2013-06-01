//*** Font.cpp **

#include "Font.h"
#include "Image.h"
#include "StandardLibrary.h"
#include "Bitmap_RLE8.h"
#include "ColorHelper.h"
#include "Asset.h"
#include "Bitmap.h"
#include "Filename.h"


//*** Constructor ***

Font::Font():
	size_(0),
	lineSpacing_(0),
	baseline_(0),
	layerCount_(0)
	{
	}


//*** Constructor ***

Font::Font(const Asset& asset):
	size_(0),
	lineSpacing_(0),
	baseline_(0),
	layerCount_(0)
	{
	if (asset.Open())
		{
		char header[8];
		asset.Read(header,8);
		if (StrNCmp(header,"PIXIE_FN",8)==0)
			{
			char h;
			asset.Read(&h);
			Assert(h=='T',"Invalid header");
			if (h!='T')
				{
				asset.Close();
				return;
				}
			int version=0;
			asset.Read(&version);
			if (version==0)
				{
				ReadFromAsset(&asset);
				}
			}
		else if (StrNCmp(header,"FNTRLE8B",8)==0)
			{
			int version=0;
			asset.Read(&version);
			if (version==0)
				{
				ReadFromAssetNew(&asset);
				}
			}
		else
			{
			Assert(false,"Invalid header");
			}
		asset.Close();
		}
	// Report missing file
	#ifdef _DEBUG
	else
		{
		const char* filename=asset.GetFilename().GetString();
		if (filename)
			{
			char errorMessage[1024];
			SNPrintF(errorMessage,1024,"File not found: %s",filename);
			Assert(false,errorMessage);
			}
		else
			{
			Assert(false,"An asset could not be accessed.");
			}
		}
	#endif
	}


//*** Constructor ***

Font::Font(const Filename& filename):
	size_(0),
	lineSpacing_(0),
	baseline_(0),
	layerCount_(0)
	{
	LoadFont(filename);
	}


//*** LoadFont ***

void Font::LoadFont(const Filename& filename)
	{
	int length=StrLen(filename.GetString());
	if (ToLower(filename.GetString()[length-3])=='f' && ToLower(filename.GetString()[length-2])=='n' && ToLower(filename.GetString()[length-1])=='t')
		{
		Asset asset(filename);
		if (asset.Open())
			{
			char header[8];
			asset.Read(header,8);
			if (StrNCmp(header,"PIXIE_FN",8)==0)
				{
				char h;
				asset.Read(&h);
				Assert(h=='T',"Invalid header");
				if (h!='T')
					{
					asset.Close();
					return;
					}
				int version=0;
				asset.Read(&version);
				if (version==0)
					{
					ReadFromAsset(&asset);
					}
				}
			else if (StrNCmp(header,"FNTRLE8B",8)==0)
				{
				int version=0;
				asset.Read(&version);
				if (version==0)
					{
					ReadFromAssetNew(&asset);
					}
				}
			else
				{
				Assert(false,"Invalid header");
				}
			asset.Close();
			}
		// Report missing file
		#ifdef _DEBUG
		else
			{
			const char* filename=asset.GetFilename().GetString();
			if (filename)
				{
				char errorMessage[1024];
				SNPrintF(errorMessage,1024,"File not found: %s",filename);
				Assert(false,errorMessage);
				}
			else
				{
				Assert(false,"An asset could not be accessed.");
				}
			}
		#endif
		return;
		}

	// Load font definition
	LoadXML(filename);

	layerCount_=layers_.GetItemCount();

	// Find path
	const char* l1=StrRChr(filename.GetString(),'/');
	const char* l2=StrRChr(filename.GetString(),'\\');
	const char* l=l1;
	if (l2>l)
		{
		l=l2;
		}
	char path[1024];
	if (l>0)
		{
		int len=(int)(l-filename.GetString()+1);
		StrNCpy(path,filename.GetString(),len);
		path[len]=0;
		}
	else
		{
		StrCpy(path,"");
		}
    

	// Create RLE bitmaps
	for (int i=0; i<layers_.GetItemCount(); i++)
		{
		const char* filename=layers_.Get(i);
		char filenameFullPath[1024];
		SNPrintF(filenameFullPath,1024,"%s%s",path,filename);
		Filename fn(filenameFullPath);
		Image layer(fn);
		
		for (int i=0; i<256; i++)
			{
			Character& character=characters_[i];
			if (!character.isBlank && character.width>0 && character.height>0)
				{
				Image image(character.width,character.height);
				for (int y=0; y<character.height; y++)
					{
					for (int x=0; x<character.width; x++)
						{
						unsigned int pixel = layer.GetPixel(character.x+x,character.y+y);
						image.SetPixel(x,y,pixel);
						}
					}
				Bitmap* bitmap= new Bitmap_RLE8(image);
				character.layers.Add(bitmap);

				}
			else
				{
				character.isBlank=true;
				}
			}
		}

	// Delete layer names
	for (int i=0; i<layers_.GetItemCount(); i++)
		{
		char* filename=layers_.Get(i);
		delete filename;
		}
	layers_.Clear(true);
	}


//*** Destructor ***

Font::~Font()
	{
	for (int i=0; i<256; i++)
		{
		Character& character=characters_[i];
		for (int i=0; i<character.layers.GetItemCount(); i++)
			{
			Bitmap* bitmap=character.layers.Get(i);
			delete bitmap;
			}
		}
	}


//*** GetCharacter ***

const Font::Character& Font::GetCharacter(int ascii) const
	{
	if (size_==0 || ascii<0 || ascii>255)
		{
		static Character defaultValue;
		return defaultValue;
		}
	
	return characters_[ascii];
	}


//*** Blit ***

void Font::Blit(Bitmap& bitmap, int x, int y, const char* text, int spacing, unsigned short modulate, unsigned char alpha) const
	{
	if (size_==0)
		{
		return;
		}
	int textLength=StrLen(text);
	int previousCharacter=0;
	for (int i=0; i<layerCount_; i++)
		{	
		int xp=x;
		for (int j=0; j<textLength; j++)
			{	
			const Character& character=GetCharacter(text[j]);
			int kerning=0;
			if (j>0)
				{
				kerning=GetKerning(text[j-1],text[j]);
				}
			if (!character.isBlank && character.layers.GetItemCount()>i)
				{
				Bitmap* charBitmap=character.layers.Get(i);
				charBitmap->Blit(bitmap,xp+character.xoffset+kerning,y+character.yoffset,modulate,alpha);
				}

			xp+=character.spacing+spacing+kerning;
			}
		}
	}


//*** GetKerning ***

int Font::GetKerning(int first, int second) const
	{
	if (size_==0 || first<0 || first>255 || second<0 || second>255)
		{
		return 0;
		}

	const Kerning& kerning=kerning_[first];
	if (kerning.first==first && kerning.second==second)
		{
		return kerning.amount;
		}

	return 0;
	}


//**** WillWordFit ***

bool Font::WillWordFit(int xp, int maxX, const char* text, int hspacing) const
	{
	if (size_==0)
		{
		return false;
		}

	int i=0;
	while (text[i]!=0 && text[i]!=' ')
		{
		const Character& character=GetCharacter(text[i]);
		xp+=character.spacing+hspacing;
		if (xp>=maxX)
			{
			return false;
			}
		i++;
		}

	return xp<maxX;
	}


//*** BlitWrap ***

void Font::BlitWrap(Bitmap& bitmap, int x, int y, const char* text, int width, int hspacing, int vspacing, unsigned short modulate, unsigned char alpha) const
	{
	if (size_==0)
		{
		return;
		}

	int textLength=StrLen(text);
	for (int i=0; i<layerCount_; i++)
		{	
		int xp=x;
		int yp=y;
		for (int j=0; j<textLength; j++)
			{	
			if (text[j]=='\n' || xp>=x+width)
				{
				xp=x;
				yp+=size_+vspacing;
				}

			// Will next word fit?
			if (text[j]==' ')
				{
				if (!WillWordFit(xp,x+width,&text[j+1],hspacing))
					{
					xp=x;
					yp+=size_+vspacing;
					continue;
					}
				}

			const Character& character=GetCharacter(text[j]);
			if (!character.isBlank && character.layers.GetItemCount()>i)
				{
				Bitmap* charBitmap=character.layers.Get(i);
				charBitmap->Blit(bitmap,xp+character.xoffset,yp+character.yoffset,modulate,alpha);
				}

				xp+=character.spacing+hspacing;
			}
		}
	}

//*** GetBaseline ***

int Font::GetBaseline() const
	{
	return baseline_;
	}


//*** GetSize ***

int Font::GetSize() const
	{
	return size_;
	}


//*** GetGlyphHeight ***

int Font::GetGlyphHeight() const
	{
	return lineSpacing_;
	}

//*** GetBounds ***

Font::Bounds Font::GetBounds(const char* text, int spacing) const
	{
	if (size_==0)
		{
		Bounds bounds;
		bounds.width=0;
		bounds.height=0;

		return bounds;
		}

	int textLength=StrLen(text);
	int xp=0;
	for (int j=0; j<textLength; j++)
		{	
		const Character& character=GetCharacter(text[j]);
		int kerning=0;
		if (j>0)
			{
			kerning=GetKerning(text[j-1],text[j]);
			}

		xp+=character.spacing+spacing+kerning;
		}
	
	Bounds bounds;
	bounds.width=xp;
	bounds.height=lineSpacing_+1;

	return bounds;
	}


//*** GetBoundsWrap ***

Font::Bounds Font::GetBoundsWrap(const char* text, int width, int hspacing, int vspacing) const
	{
	if (size_==0)
		{
		Bounds bounds;
		bounds.width=0;
		bounds.height=0;

		return bounds;
		}

	int textLength=StrLen(text);
	int xp=0;
	int yp=0;
	for (int j=0; j<textLength; j++)
		{	
		if (text[j]=='\n' || xp>=width)
			{
			xp=0;
			yp+=size_+vspacing;
			}

		// Will next word fit?
		if (text[j]==' ')
			{
			if (!WillWordFit(xp,width,&text[j+1],hspacing))
				{
				xp=0;
				yp+=size_+vspacing;
				continue;
				}
			}

		const Character& character=GetCharacter(text[j]);
		
		xp+=character.spacing+hspacing;
		}

	Bounds bounds;
	bounds.width=width;
	bounds.height=yp+size_+vspacing;

	return bounds;
	}


//*** XML_Element ***

XMLObject* Font::XML_Element(StringId _name, const XMLAttributeList& _attributes)
	{
	xmlCase(Character)
		{
		int ascii=xmlAttributeValue("ascii");
		if (ascii>=0 && ascii<=255)
			{
			Character& character=characters_[ascii];
			character.ascii=ascii;
			character.spacing=xmlAttributeValue("spacing");
			character.x=xmlAttributeValue("x");
			character.y=xmlAttributeValue("y");
			character.width=xmlAttributeValue("width");
			character.height=xmlAttributeValue("height");
			character.xoffset=xmlAttributeValue("xoffset");
			character.yoffset=xmlAttributeValue("yoffset");
			character.isBlank=xmlAttributeValue("blank");
			}
		}

	xmlCase(Layer)
		{
		char* filename=StrDup(xmlAttributeValue("filename"));
		layers_.Add(filename);		
		}

	return 0;
	}


//*** XML_Attribute ***

void Font::XML_Attribute(StringId _name, const XMLVariant& _value)
	{
	xmlCase(name)
		{
		name_=StringId(xmlValue);
		}

	xmlCase(size)
		{
		size_=xmlValue;
		}

	xmlCase(lineSpacing)
		{
		lineSpacing_=xmlValue;
		}
	}


//*** ReadFromAsset ***

void Font::ReadFromAsset(const Asset* asset)
	{
	int nameLength=0;
	asset->Read(&nameLength);
	char* name=new char[nameLength+1];
	asset->Read(name,nameLength);
	name[nameLength]=0;
	name_=StringId(name);
	delete name;
	asset->Read(&size_);
	asset->Read(&lineSpacing_);
	asset->Read(&layerCount_);
	int characterCount=0;
	asset->Read(&characterCount);

	for (int i=0; i<characterCount; i++)
		{
		Character character;
		asset->Read(&character.ascii);
		asset->Read(&character.spacing);
		asset->Read(&character.x);
		asset->Read(&character.y);
		asset->Read(&character.width);
		asset->Read(&character.height);
		asset->Read(&character.xoffset);
		asset->Read(&character.yoffset);
		character.yoffset=-character.yoffset;
		asset->Read(&character.isBlank);

		if (!character.isBlank)
			{
			for (int i=0; i<layerCount_; i++)
				{
				Bitmap* bitmap=new Bitmap_RLE8();
				bitmap->ReadFromAsset(asset);
				character.layers.Add(bitmap);
				}
			}

		if (character.ascii>=0 && character.ascii<=255)
			{
			characters_[character.ascii]=character;
			}
		}
	}


//*** WriteToAsset ***

void Font::WriteToAsset(Asset* asset)
	{
	int nameLength=StrLen(name_.GetString());
	asset->Write(&nameLength);
	asset->Write(name_.GetString(),nameLength);
	asset->Write(&size_);
	asset->Write(&lineSpacing_);
	asset->Write(&layerCount_);
	int characterCount=256;
	asset->Write(&characterCount);

	for (int i=0; i<256; i++)
		{
		const Character& character=characters_[i];
		asset->Write(&i);
		asset->Write(&character.spacing);
		asset->Write(&character.x);
		asset->Write(&character.y);
		asset->Write(&character.width);
		asset->Write(&character.height);
		asset->Write(&character.xoffset);
		asset->Write(&character.yoffset);
		asset->Write(&character.isBlank);

		if (!character.isBlank)
			{
			for (int i=0; i<layerCount_; i++)
				{
				character.layers.Get(i)->WriteToAsset(asset);
				}
			}
		
		}

	}


//*** ReadFromAssetNew ***

void Font::ReadFromAssetNew(const Asset* asset)
	{
	int nameLength=0;
	asset->Read(&nameLength);
	char* name=new char[nameLength+1];
	asset->Read(name,nameLength);
	name[nameLength]=0;
	name_=StringId(name);
	delete name;
	asset->Read(&size_);
	asset->Read(&lineSpacing_);
	asset->Read(&baseline_);
	asset->Read(&layerCount_);
	int characterCount=0;
	asset->Read(&characterCount);

	for (int i=0; i<characterCount; i++)
		{
		Character character;
		character.isBlank=false;
		asset->Read(&character.ascii);
		asset->Read(&character.spacing);
		asset->Read(&character.xoffset);
		asset->Read(&character.yoffset);

		for (int i=0; i<layerCount_; i++)
			{
			Bitmap* bitmap=new Bitmap_RLE8();
			bitmap->ReadFromAsset(asset);
			if (character.ascii>=0 && character.ascii<=255)
				{
				character.layers.Add(bitmap);
				}
			else
				{
				delete bitmap;
				}

			}

		if (character.ascii>=0 && character.ascii<=255)
			{
			characters_[character.ascii]=character;
			}
		}

	int kerningCount=0;
	asset->Read(&kerningCount);

	for (int i=0; i<kerningCount; i++)
		{
		Kerning kerning;
		asset->Read(&kerning.first);
		asset->Read(&kerning.second);
		asset->Read(&kerning.amount);
		
		if (kerning.first>=0 && kerning.first<=255)
			{
			kerning_[kerning.first]=kerning;
			}
		}
	}
