/**
 * \class	PathFinderCell
 * 
 * \ingroup	ai
 * \brief	
 * \author	Mattias Gustavsson	
 *
 *
 */

//#ifndef __PathFinderCell_H__
//#define __PathFinderCell_H__
//
//// Includes
//
//// Forward declares
//class PathFinderAgent;
//
//// PathFinderCell
//class PathFinderCell
//	{
//	public:
//		/**
//		 * This function returns the cost of moving the specified agent from this cell
//		 * to the specified cell, and requires the start and end cells to be adjacent
//		 */
//		virtual float CalculateCost(
//			const PathFinderAgent* agent, 
//			const PathFinderCell* to
//			) const = 0;
//
//		/**
//		 * This function returns an estimate of how much it would cost to move  the specified 
//		 * agent from this cell to the specified cell, and does not requires the start  and 
//		 * end cells to be adjacent
//		 */
//		virtual float EstimateCost(
//			const PathFinderAgent* agent, 
//			const PathFinderCell* to
//			) const = 0;
//
//		/** 
//		 * This function returns the number of cells adjacent to this cell.
//		 * For a normal tilebased game, it's usually 4, for a hex game it'd be 6.
//		 */
//		virtual int GetNeighbourCount(
//			const PathFinderAgent* agent
//			) const = 0;
//
//		/** 
//		 * This function returns one of the neighbours of this cell (which 
//		 * one depends on the index)
//		 */
//		virtual const PathFinderCell* GetNeighbour(
//			const PathFinderAgent* agent, 
//			int index
//			) const = 0;
//	
//	};
//
//#endif // __PathFinderCell_H__
