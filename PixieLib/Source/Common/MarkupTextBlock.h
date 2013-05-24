//*** MarkupTextBlock.h ***

#ifndef __MarkupTextBlock_H__
#define __MarkupTextBlock_H__

// Includes
#include "Array.h"
#include "StringId.h"

// Forward declares
class Bitmap;

// TextPML
class MarkupTextBlock
	{
	public:
		MarkupTextBlock();

		MarkupTextBlock(
			const char* pmlString
			);

		~MarkupTextBlock();

		MarkupTextBlock(
			const MarkupTextBlock& textblock
			);

		const MarkupTextBlock& operator=(
			const MarkupTextBlock& textblock
			);

		void SetString(
			const char* pmlString
			);

		void AddString(
			const char* pmlString
			);

		const char* GetString() const;

		void Render(
			Bitmap& target, 
			int x1, 
			int y1, 
			int x2, 
			int y2,
			StringId hoverOption,
			StringId hoverLink
			);

		StringId IsOverOption(int cursorX, int cursorY) const;
		StringId IsOverLink(int cursorX, int cursorY) const;

	private:
		void ResetTokenStream();
		const char* GetNextToken();
		

	private:
		char* pmlString_;
		char* tokenBuffer_;
		bool currentTokenIsMarkupCode_;
		int currentTokenPosition_;
		int tokenStreamSize_;

		struct InteractiveRegion
			{
			int x1;
			int y1;
			int x2;
			int y2;
			StringId id;
			};
		Array<InteractiveRegion> links_;
		Array<InteractiveRegion> options_;
	};


#endif /* __MarkupTextBlock_H__ */
