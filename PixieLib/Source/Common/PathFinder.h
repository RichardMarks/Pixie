/**
 * \class	PathFinder
 * 
 * \ingroup	ai
 * \brief	Generic A* implementation
 * \author	Mattias Gustavsson	
 *
 * This class implements a generic pathfinder using the A* algorithm.
 * It allows the caller to specify methods for calculating and estimating
 * costs of movement, as well as enumerating neighbour cells. This means
 * it can find paths within any type of connected cells, be it geographical
 * paths or problem solving paths.
 *
 * For flexibility, agents and cells are defined as void pointers, ad it
 * is up to the calling code to map these pointers to its appropriate
 * internal datastructures.
 *
 */

#ifndef __PathFinder_H__
#define __PathFinder_H__

// Includes
#include "HashTable.h"
#include "HashTableKey_Pointer.h"
#include "MemoryPool.h"
#include "PriorityQueue.h"
#include "Array.h"

// Forward declares


// PathFinder
template <class CELL>
class PathFinder
	{
	public:
		PathFinder();

		/**
		 * Attempts to find a path from one cell to another
		 *
		 * \returns	True if a path was found, False if not
		 */
		template<class AGENT> bool FindPath(
			const AGENT* agent,	///< The agent that is to travel along the path
			const CELL* start,	///< The start cell of the path
			const CELL* end,		///< The desired goal cell of the path
			Array<const CELL*>& path, ///< Array that will hold the resulting path, if one was found
			int iterationSafetyLimit=80000 ///< Maximum number of iterations before aborting pathfinding. This is a safety limit, to avoid having the program hang for too long. Specify 0 for no limit.
			);			
		

	private:
		/**
		 * This is used internally to keep track of the progress of the pathfinding.
		 */
		struct PathFinderNode
			{
			float costFromStart; ///< The cost of getting to this node from the start node
			float estimatedCostToGoal; ///< A heuristic estimate of the cost of getting to the goal node from this node
			float nodeScore; ///< An overall score of this node, used when ordering nodes in a priority queue
			PathFinderNode* parent; ///< A pointer to the node we visited before this one
			const CELL* cell; ///< A pointer to the cell that this node represents
			bool closed; ///< Is set to true when the node is moved to the closed list
			};

	private:
		/**
		 * Clears the Open and Closed lists, and returns all allocated elements in the main
		 * node list to the memory pool
		 */
		void ClearLists();

		/**
		 * Looks through the list of nodes, and if it finds one with the specified cell,
		 * it returns it. If a node with the specified cell is not found, it is created
		 * and inserted into the list.
		 *
		 * \returns	A node for the specified cell
		 */
		PathFinderNode*	GetNodeFromCell(const CELL* cell);

		/**
		 * Once a path have been found, this function will be called, which will construct a
		 * path from the stored node cell. The result will be put into the array
		 * specified by the caller, and will consist of an array of pointers to cells.
		 */
		void BuildPath(PathFinderNode* node,Array<const CELL*>& path);

		class PriorityCompare
			{
			public:
				static bool Compare(typename PathFinderNode* const &a, PathFinderNode* const &b);
			};

	private:
		PriorityQueue<PathFinderNode*,PriorityCompare> openNodes_; // List of "open" nodes
		HashTable<HashTableKey_Pointer,PathFinderNode*> nodes_; // List of all nodes that this pathfinder uses
		MemoryPool<PathFinderNode> memoryPool_; // Memory pool used to speed up dynamic allocation
	
	};

// Implementation
#include "PathFinder.inl"

#endif // __PathFinder_H__
