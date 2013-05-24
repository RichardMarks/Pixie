/**
 * \class	AssetEnumerator
 * 
 * \ingroup	core
 * \brief	
 * \author	Mattias Gustavsson	
 * 
 */

#ifndef __AssetEnumerator_H__
#define __AssetEnumerator_H__

// Includes

// Forward declares
class Platform_FileSystem_Directory;
class ArchiveDirectory;
class Filename;
class StringId;

// AssetEnumerator
class AssetEnumerator
	{
	public:
		/**
		 * Constructor
		 */
		AssetEnumerator(const Filename& path);
		AssetEnumerator(const StringId& path);
		AssetEnumerator(const char* path);

		/**
		 * Destructor
		 */
		~AssetEnumerator();

		/**
		 * Copy Constructor
		 */
		AssetEnumerator(
			const AssetEnumerator& assetEnumerator
			);
		
		/**
		 * Copy Operator
		 */
		const AssetEnumerator& operator=(
			const AssetEnumerator& assetEnumerator
			);

		/**
		 *
		 */
		int GetAssetCount();

		/**
		 *
		 */
		const char* GetAssetFilename(
			int index
			);


		/**
		 *
		 */
		int GetSubdirectoryCount();

		/**
		 *
		 */
		const char* GetSubdirectoryName(
			int index
			);

	private:
		void CreateFromFilename(const Filename& filename);
	private:
		Platform_FileSystem_Directory* directory_;		
		const ArchiveDirectory* archiveDirectory_;
		char* nameBuffer_;
		int nameBufferSize_;
	};


#endif /* __AssetEnumerator_H__ */
