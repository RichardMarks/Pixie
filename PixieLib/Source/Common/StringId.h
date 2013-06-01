/**
 * \class	StringId
 * 
 * \ingroup	core
 * \brief	Lightweight type for storing strings for use as ID's
 * \author	Mattias Gustavsson	
 * 
 * The StringId type is perfect for when you need to identify something by name. When
 * you create a StringId type from a c-style zero-terminated string (const char*), it
 * will calculate a hash-number from all the characters, and use the resulting value
 * as an index into a global array of string id data. This array entry typically holds
 * just the string we're looking for (or is empty in case this is the first time a 
 * StringId is requested for that particular string), but sometimes it can hold a list
 * of just a few strings that happen to have the same hash-number. Once the corresonding
 * string have been found (or inserted, if this is its first use) in the table, it is
 * stored in the StringId type.
 *
 * This initial string matching process involves looping through each character of the
 * string, and (worst case) a handful of string compares), which isn't exactly crazy 
 * expensive, but certainly not free, either. All other operations you perform on a
 * StringId is extremely cheap though. Since the string pointer (const char*) stored
 * inside the StringId type is from the shared string id table, we can compare two
 * StringId values by simply comparing two pointers. And we can copy a StringId by simply
 * copying its internal pointer - no extra lookup needed.
 *
 * So we trade a little bit of overhead at creation (looping through the string once and
 * maybe doing a couple of string compares) for almost free copying and comparison when
 * using it. If you just make sure not to unnecessarily create StringId's, you won't ever
 * find them slowing things down.
 *
 * There's also a couple of macros, strSwitch and strCase, which you can use to emulate
 * the behaviour of the c-language statements "switch" and "case", to make it more easy
 * to do a bunch of comparisons for a single StringId. The strCase macro is written such
 * that it automatically initializes the specified StringId once on the first time it is
 * run, meaning it will always just do a pointer compare on subsequent runs.
 */

#ifndef __StringId_H__
#define __StringId_H__

// Includes

// Forward declares

// StringId
class StringId
	{
	public:
		/**
		 * Default constructor. Will initialize the internal string to 0, without 
		 * doing any lookups. This is a fast operation.
		 */
		StringId();

		/**
		 * Constructor. Looks up the specified string in the shared string table, and
		 * calculates the hash value for the string. If it is the empty string (""), 
		 * the StringId will use a string pointer of 0 instead. This is so that both
		 * empty strings and null-strings is treated like the same ID, which is more
		 * useful. 
		 */
		StringId(
			const char* idString	///< The string to use for identifier
			);

		/**
		 * Used to retrieve the original, c-style string (const char*) for this StringId.
		 *
		 * \returns The zero-terminated string for this id
		 */
		const char* GetString() const;
			

		/**
		 * Retrieve the hash-number calculated for the string. This number is primarily 
		 * for internal use, but can also be useful if you want to store StringId's in
		 * your own hash-table, as you could reuse this stored hash-number instead of
		 * having to recalculate it yourself. The number is calculated in such a way as
		 * to avoid collisions, but collisions WILL happen anyway, so must be dealt with
		 *
		 * \returns The hash-number for the string
		 */
		unsigned int GetHash() const;


		/**
		 * Copy constructor. Just duplicates the internally stored hash value and string
		 * pointer, without doing any lookups.
		 */
		StringId(
			const StringId& stringId	///< StringId to copy
			);

		/**
		 * Assignment operator. Just duplicates the internally stored hash value and 
		 * string pointer, without doing any lookups.
		 */
		const StringId& operator=(
			const StringId& stringId	///< StringId to copy
			);

		/**
		 * Comparison operation (equality). Internally, this just compares two pointers,
		 * with no lookups taking place, so is really fast.
		 */
		bool operator==(
			const StringId& stringId
			) const;

		/**
		 * Comparison operation (inequality). Internally, this just compares two 
		 * pointers, with no lookups taking place, so is really fast.
		 */
		bool operator!=(
			const StringId& stringId
			) const;


	private:
		const char* idString_;	///< Pointer to the (shared) string for this id
	};


//*** Helper macros for working with StringIds ***

/**
 * \ingroup core
 * \brief Macro to emulate the behavior of the switch-statement for StringIds
 *
 * Macro to emulate the behavior of the switch-statement for StringIds
 * Used in conjunction with the strCase macro.
 *
 * Example use:
 *
 *		strSwitch (myFruitTypeStringId)
 *			{
 *			strCase(Apples)
 *				{
 *				// code to do stuff here
 *				}
 *
 *			strCase(Oranges)
 *				{
 *				// code to do other stuff here
 *				}
 *			}
 *
 * The above is the equivalent of doing:
 *
 *		static StringId applesId("Apples");
 *		if (myFruitTypeStringId==applesId)
 *			{
 *			// code to do stuff here
 *			}
 *
 *		static StringId orangesId("Oranges");
 *		if (myFruitTypeStringId==orangesId)
 *			{
 *			// code to do other stuff here
 *			}
 *
 * Note that you can't have to strSwitch blocks within the same code block, and you
 * can't test for the same strCase more than once in the same strSwitch. The internal
 * variable names of the macros have been chosen to indicate these errors in case it
 * is attempted accidentally.
 */
#define strSwitch(stringId) StringId _multiple_strSwitch_not_allowed_within_one_code_block_=stringId;


/**
 * \ingroup core
 * \brief Macro to emulate the behavior of the case-statement for StringIds
 *
 * Macro to emulate the behavior of the case-statement for StringIds
 * Used in conjunction with the strSwitch macro (see strSwitch for further info)
 */
#define strCase(stringId)																					\
	static StringId _duplicate_strCase_statement_##stringId(#stringId);										\
	if (_duplicate_strCase_statement_##stringId==_multiple_strSwitch_not_allowed_within_one_code_block_)	\


#endif /* __StringId_H__ */
