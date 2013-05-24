/**
 * \class	ResourceManager
 * 
 * \ingroup	resources
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __ResourceManager_H__
#define __ResourceManager_H__

// Includes
#include "Singleton.h"
#include "StringId.h"
#include "HashTable.h"
#include "HashTableKey_Pointer.h"

// Forward declares
class BitmapStrip;
class Font;
class Sound;
class Image;
class Filename;
class Bitmap;


// ResourceManager
class ResourceManager:public Singleton<ResourceManager>
	{
	public:
		ResourceManager();
		virtual ~ResourceManager();

		void LoadBitmapStrip(
			const Filename& filename,
			int celCount = 1
			);

		void UnloadBitmapStrip(
			const Filename& filename
			);
	
		void LoadFont(
			const Filename& filename
			);

		void UnloadFont(
			const Filename& filename
			);

		void LoadSound(
			const Filename& filename
			);

		void UnloadSound(
			const Filename& filename
			);
	private:
		friend class Resource_BitmapStrip;
		friend class Resource_Font;
		friend class Resource_Sound;

		const BitmapStrip* GetBitmapStrip(
			StringId filename,
			int celCount = 1
			);

		const BitmapStrip* GetBitmapStrip(
			const Image& image
			);
	
		const BitmapStrip* GetBitmapStrip(
			const Bitmap* bitmap
			);

		const Font* GetFont(
			StringId filename
			);

		const Sound* GetSound(
			StringId filename
			);

		void IncreaseReferenceCount(
			const BitmapStrip* bitmapStrip
			);
		
		void DecreaseReferenceCount(
			const BitmapStrip* bitmapStrip
			);

		void IncreaseReferenceCount(
			const Font* font
			);
		
		void DecreaseReferenceCount(
			const Font* font
			);

		void IncreaseReferenceCount(
			const Sound* sound
			);
		
		void DecreaseReferenceCount(
			const Sound* sound
			);

		struct BitmapStripEntry
			{
			StringId filename;
			const BitmapStrip* bitmapStrip;
			int referenceCount;
			};

		BitmapStripEntry* GetBitmapStripEntry(
			const BitmapStrip* bitmapStrip
			);

		struct FontEntry
			{
			StringId filename;
			const Font* font;
			int referenceCount;
			};

		FontEntry* GetFontEntry(
			const Font* font
			);

		struct SoundEntry
			{
			StringId filename;
			const Sound* sound;
			int referenceCount;
			};

		SoundEntry* GetSoundEntry(
			const Sound* sound
			);


	private:
		HashTable<HashTableKey_Pointer,BitmapStripEntry*> bitmapStrips_;
		HashTable<HashTableKey_Pointer,FontEntry*> fonts_;
		HashTable<HashTableKey_Pointer,SoundEntry*> sounds_;
	};

#define siResourceManager ResourceManager::GetInstance()

#endif /* __ResourceManager_H__ */

	