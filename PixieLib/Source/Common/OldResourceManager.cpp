//*** OldResourceManager.cpp ***

#include "OldResourceManager.h"
#include "Debug.h"
#include "BitmapStrip.h"
#include "Bitmap.h"
#include "Bitmap_RLE8.h"
#include "Bitmap_16Bit.h"
#include "Bitmap_16BitAlpha.h"
#include "Bitmap_16BitAlphaCrop.h"
#include "Bitmap_Alpha.h"
#include "Image.h"
#include "Asset.h"
#include "HashTableIterator.h"
#include "Font.h"
#include "Sound.h"
#include "Filename.h"


//*** Constructor ***

OldResourceManager::OldResourceManager()
	{
	}


//*** Destructor ***

OldResourceManager::~OldResourceManager()
	{
	// Delete all bitmap strips
	HashTableIterator<HashTableKey_Pointer,BitmapStripEntry*> itBS(bitmapStrips_);
	while(itBS.IsValid())
		{
		BitmapStripEntry* bitmapStripEntry=itBS.GetCurrent();
		delete bitmapStripEntry->bitmapStrip;
		delete bitmapStripEntry;
		itBS.MoveNext();
		}


	// Delete all fonts
	HashTableIterator<HashTableKey_Pointer,FontEntry*> itF(fonts_);
	while(itF.IsValid())
		{
		FontEntry* fontEntry=itF.GetCurrent();
		delete fontEntry->font;
		delete fontEntry;
		itF.MoveNext();
		}

	// Delete all sounds
	HashTableIterator<HashTableKey_Pointer,SoundEntry*> itS(sounds_);
	while(itS.IsValid())
		{
		SoundEntry* soundEntry=itS.GetCurrent();
		delete soundEntry->sound;
		delete soundEntry;
		itS.MoveNext();
		}
	}


//*** GetBitmapStripEntry ***

OldResourceManager::BitmapStripEntry* OldResourceManager::GetBitmapStripEntry(const BitmapStrip* bitmapStrip)
	{
	HashTableIterator<HashTableKey_Pointer,BitmapStripEntry*> it(bitmapStrips_);
	if (it.Find(HashTableKey_Pointer(bitmapStrip)))
		{
		return it.GetCurrent();
		}

	BitmapStripEntry* entry=new BitmapStripEntry();
	entry->filename=0;
	entry->bitmapStrip=bitmapStrip;
	entry->referenceCount=0;
	bitmapStrips_.Insert(HashTableKey_Pointer(bitmapStrip),entry);
	return entry;
	}



//*** GetFontEntry ***

OldResourceManager::FontEntry* OldResourceManager::GetFontEntry(const Font* font)
	{
	HashTableIterator<HashTableKey_Pointer,FontEntry*> it(fonts_);
	if (it.Find(HashTableKey_Pointer(font)))
		{
		return it.GetCurrent();
		}

	FontEntry* entry=new FontEntry();
	entry->filename=0;
	entry->font=font;
	entry->referenceCount=0;
	fonts_.Insert(HashTableKey_Pointer(font),entry);
	return entry;
	}


//*** GetSoundEntry ***

OldResourceManager::SoundEntry* OldResourceManager::GetSoundEntry(const Sound* sound)
	{
	HashTableIterator<HashTableKey_Pointer,SoundEntry*> it(sounds_);
	if (it.Find(HashTableKey_Pointer(sound)))
		{
		return it.GetCurrent();
		}

	SoundEntry* entry=new SoundEntry();
	entry->filename=0;
	entry->sound=sound;
	entry->referenceCount=0;
	sounds_.Insert(HashTableKey_Pointer(sound),entry);
	return entry;
	}


//*** IncreaseReferenceCount ***

void OldResourceManager::IncreaseReferenceCount(const BitmapStrip* bitmapStrip)
	{	
	BitmapStripEntry* entry=GetBitmapStripEntry(bitmapStrip);
	entry->referenceCount++;
	}


//*** DecreaseReferenceCount ***

void OldResourceManager::DecreaseReferenceCount(const BitmapStrip* bitmapStrip)
	{
	BitmapStripEntry* entry=GetBitmapStripEntry(bitmapStrip);
	Assert(entry->referenceCount>=1,"Tried to decrease reference count when already 0");
	if (entry->referenceCount<=0)
		{
		return;
		}

	entry->referenceCount--;
	
	if (entry->referenceCount==0)
		{
		bitmapStrips_.Remove(HashTableKey_Pointer(bitmapStrip));
		delete entry->bitmapStrip;
		delete entry;
		}
	}


//*** IncreaseReferenceCount ***

void OldResourceManager::IncreaseReferenceCount(const Font* font)
	{	
	FontEntry* entry=GetFontEntry(font);
	entry->referenceCount++;
	}


//*** DecreaseReferenceCount ***

void OldResourceManager::DecreaseReferenceCount(const Font* font)
	{
	FontEntry* entry=GetFontEntry(font);
	Assert(entry->referenceCount>=1,"Tried to decrease reference count when already 0");
	if (entry->referenceCount<=0)
		{
		return;
		}

	entry->referenceCount--;
	
	if (entry->referenceCount==0)
		{
		fonts_.Remove(HashTableKey_Pointer(font));
		delete entry->font;
		delete entry;
		}
	}


//*** IncreaseReferenceCount ***

void OldResourceManager::IncreaseReferenceCount(const Sound* sound)
	{	
	SoundEntry* entry=GetSoundEntry(sound);
	entry->referenceCount++;
	}


//*** DecreaseReferenceCount ***

void OldResourceManager::DecreaseReferenceCount(const Sound* sound)
	{
	SoundEntry* entry=GetSoundEntry(sound);
	Assert(entry->referenceCount>=1,"Tried to decrease reference count when already 0");
	if (entry->referenceCount<=0)
		{
		return;
		}

	entry->referenceCount--;
	
	if (entry->referenceCount==0)
		{
		sounds_.Remove(HashTableKey_Pointer(sound));
		delete entry->sound;
		delete entry;
		}
	}



//*** GetBitmapStrip ***

const BitmapStrip* OldResourceManager::GetBitmapStrip(StringId filename, int celCount)
	{
	if (!filename.GetString())
		{
		return 0;
		}
	HashTableIterator<HashTableKey_Pointer,BitmapStripEntry*> it(bitmapStrips_);
	while(it.IsValid())
		{
		BitmapStripEntry* entry=it.GetCurrent();
		if (entry->filename==filename && entry->bitmapStrip)
			{
			return entry->bitmapStrip;
			}
		it.MoveNext();
		}
	
	const BitmapStrip* bitmapStrip=0;
	int l=StrLen(filename.GetString());

	// If new style .PIX file
	if (l>3 && ToLower(filename.GetString()[l-3])=='p' && ToLower(filename.GetString()[l-2])=='i' && ToLower(filename.GetString()[l-1])=='x')
		{
		bitmapStrip=new BitmapStrip(filename);
		}
	
	// If old style .RLE file
	else if (l>3 && ToLower(filename.GetString()[l-3])=='r' && ToLower(filename.GetString()[l-2])=='l' && ToLower(filename.GetString()[l-1])=='e')
		{
		bitmapStrip=new BitmapStrip(filename);
		}
	
	// If old style .ABM file
	else if (l>3 && ToLower(filename.GetString()[l-3])=='a' && ToLower(filename.GetString()[l-2])=='b' && ToLower(filename.GetString()[l-1])=='m')
		{
		bitmapStrip=new BitmapStrip(filename);
		}

	// If old style .BM file
	else if (l>2 && ToLower(filename.GetString()[l-2])=='b' && ToLower(filename.GetString()[l-1])=='m')
		{
		bitmapStrip=new BitmapStrip(filename);
		}

	// If old style .AM file
	else if (l>2 && ToLower(filename.GetString()[l-2])=='a' && ToLower(filename.GetString()[l-1])=='m')
		{
		bitmapStrip=new BitmapStrip(filename);
		}

	// If arbitrary image format, .TGA, .GIF etc
	else
		{
		bitmapStrip=new BitmapStrip(Image(filename,celCount));
		}
	

	BitmapStripEntry* entry=GetBitmapStripEntry(bitmapStrip);
	entry->filename=filename;
	
	return entry->bitmapStrip;
	}


//*** GetBitmapStrip ***

const BitmapStrip* OldResourceManager::GetBitmapStrip(const Image& image)
	{
	const BitmapStrip* bitmapStrip=new BitmapStrip(image);

	BitmapStripEntry* entry=GetBitmapStripEntry(bitmapStrip);
	
	return entry->bitmapStrip;
	}


//*** GetBitmapStrip ***

const BitmapStrip* OldResourceManager::GetBitmapStrip(const Bitmap* bitmap)
	{
	const BitmapStrip* bitmapStrip=new BitmapStrip(bitmap);

	BitmapStripEntry* entry=GetBitmapStripEntry(bitmapStrip);
	
	return entry->bitmapStrip;
	}


//*** GetFont ***

const Font* OldResourceManager::GetFont(StringId filename)
	{
	if (!filename.GetString())
		{
		return 0;
		}
	HashTableIterator<HashTableKey_Pointer,FontEntry*> it(fonts_);
	while(it.IsValid())
		{
		FontEntry* entry=it.GetCurrent();
		if (entry->filename==filename && entry->font)
			{
			return entry->font;
			}
		it.MoveNext();
		}
	
	const Font* font=0;
	int l=StrLen(filename.GetString());
	if (ToLower(filename.GetString()[l-3])=='f' && ToLower(filename.GetString()[l-2])=='n' && ToLower(filename.GetString()[l-1])=='t')
		{
		font=new Font(Asset(filename));
		}
	else
		{
		font=new Font(Filename(filename));
		}
	

	FontEntry* entry=GetFontEntry(font);
	entry->filename=filename;
	
	return entry->font;
	}


//*** GetSound ***

const Sound* OldResourceManager::GetSound(StringId filename)
	{
	if (!filename.GetString())
		{
		return 0;
		}
	HashTableIterator<HashTableKey_Pointer,SoundEntry*> it(sounds_);
	while(it.IsValid())
		{
		SoundEntry* entry=it.GetCurrent();
		if (entry->filename==filename && entry->sound)
			{
			return entry->sound;
			}
		it.MoveNext();
		}
	
	const Sound* sound=new Sound(filename);
	
	SoundEntry* entry=GetSoundEntry(sound);
	entry->filename=filename;
	
	return entry->sound;
	}


//*** LoadBitmapStrip ***

void OldResourceManager::LoadBitmapStrip(const Filename& filename, int celCount)
	{
	const BitmapStrip* bitmapStrip=GetBitmapStrip(filename.GetStringId(),celCount);
	IncreaseReferenceCount(bitmapStrip);
	}


//*** UnloadBitmapStrip ***

void OldResourceManager::UnloadBitmapStrip(const Filename& filename)
	{
	const BitmapStrip* bitmapStrip=GetBitmapStrip(filename.GetStringId());
	DecreaseReferenceCount(bitmapStrip);
	}


//*** LoadFont ***

void OldResourceManager::LoadFont(const Filename& filename)
	{
	const Font* font=GetFont(filename.GetStringId());
	IncreaseReferenceCount(font);
	}


//*** UnloadFont ***

void OldResourceManager::UnloadFont(const Filename& filename)
	{
	const Font* font=GetFont(filename.GetStringId());
	DecreaseReferenceCount(font);
	}


//*** LoadSound ***

void OldResourceManager::LoadSound(const Filename& filename)
	{
	const Sound* sound=GetSound(filename.GetStringId());
	IncreaseReferenceCount(sound);
	}


//*** UnloadSound ***

void OldResourceManager::UnloadSound(const Filename& filename)
	{
	const Sound* sound=GetSound(filename.GetStringId());
	DecreaseReferenceCount(sound);
	}
