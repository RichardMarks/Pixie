//*** PathFinder.cpp ***

#include "PathFinder.h"
#include "MemoryPool.h"
#include "PriorityQueueIterator.h"
#include "HashTableIterator.h"

bool PathFinder::PriorityCompareFunction(PathFinder::PathFinderNode* const &a, PathFinder::PathFinderNode* const &b)
	{
	if (a->nodeScore < b->nodeScore)
		{
		return true;
		}

	return false;
	}


//*** Constructor *** 

PathFinder::PathFinder():
	memoryPool_(1024),
	openNodes_(PriorityCompareFunction,1024),
	nodes_(1024)
	{

	}


//*** GetNodeFromCell ***

PathFinder::PathFinderNode* PathFinder::GetNodeFromCell(const PathFinderCell* cell)
	{
	// See if a node already exists for this cell
	HashTableIterator<HashTableKey_Pointer,PathFinderNode*> it(nodes_);
	if (it.Find(cell))
		{
		// Yes, it exists, so return it
		return it.GetCurrent();
		}

	/// The node does not already exist, so create it, set it up and return it
	PathFinderNode* newnode = memoryPool_.Allocate();
	newnode->costFromStart=0;
	newnode->estimatedCostToGoal=0;
	newnode->nodeScore=0;
	newnode->parent=0;
	newnode->cell=cell;
	newnode->closed=false;
	nodes_.Insert(HashTableKey_Pointer(cell),newnode);
	return newnode;
	}



//*** BuildPath ***

void PathFinder::BuildPath(PathFinderNode* node, Array<const PathFinderCell*>& path)
	{
	// Clear any existing path that might be stored
	path.Clear();

	// Walk backwards through the nodes, adding each cell to the resulting path
	while (node)
		{
		path.Add(node->cell);
		node=node->parent;
		}

	// Clear the internal lists of nodes
	ClearLists();
	}


//*** FindPath ***

bool PathFinder::FindPath(const PathFinderAgent* agent, const PathFinderCell* start, const PathFinderCell* end, Array<const PathFinderCell*>& path, int iterationSafetyLimit)
	{
	Assert(start,"Invalid start node");
	Assert(end,"Invalid end node");
	if (!start || !end)
		{
		return false;
		}

	// Prepare start node
	PathFinderNode* startnode=GetNodeFromCell(start);
	startnode->costFromStart=0;
	startnode->estimatedCostToGoal=start->EstimateCost(agent,end);
	startnode->nodeScore=startnode->costFromStart+startnode->estimatedCostToGoal;
	openNodes_.Add(startnode);

	// Store the best node so far	
	PathFinderNode* bestNode=startnode;

	// Try all nodes on the "open" list
	int iterationCount=iterationSafetyLimit;
	while (openNodes_.GetItemCount() && (iterationSafetyLimit==0 || iterationCount>0))
		{
		// Get top node of priority queue
		PathFinderNode* n=openNodes_.Remove();
		Assert(n,"Invalid node!");
		if (!n)
			{
			return false;
			}

		// Mark node as being on the closed list
		n->closed=true;

		// Reevaluate best node so far
		if (n->estimatedCostToGoal < bestNode->estimatedCostToGoal)
			{
			bestNode = n;
			}
		
		// Check if this is the target node
		if (n->cell==end)
			{
			// If so, build the path from this node back to the start
			BuildPath(n,path);

			// Return success
			return true;
			}
		
		// Go through all the neighbours
		for (int i=0; i <n->cell->GetNeighbourCount(agent); i++)
			{
			const PathFinderCell* ncell=n->cell->GetNeighbour(agent, i);

			// Skip if node data is not valid (means no neighbour)
			if (!ncell)
				{
				continue;
				}
			
			// Examine the neighbour node
			PathFinderNode* nn = GetNodeFromCell(ncell); // The node itself


			// If node is on closed list, then skip it
			if (nn->closed)
				{
				continue;
				}

			float nnCost = n->cell->CalculateCost(agent,nn->cell); // Cost to get from current node to neighbour				
			float nnTotalCost = n->costFromStart + nnCost; // Total cost from start to get to this node

			PriorityQueueIterator<PathFinderNode*> it(openNodes_);
			int nnIndex=-1;
			if (it.Find(nn))
				{
				nnIndex=it.GetCurrentIndex();
				}


			// If node is already on open list with a lower cost, then skip it
			if ((nnIndex>=0) && nn->costFromStart <= nnTotalCost)
				{
				continue;
				}

			// Set node values
			nn->parent=n;
			nn->costFromStart=nnTotalCost;
			nn->estimatedCostToGoal=nn->cell->EstimateCost(agent,end);
			nn->nodeScore = nn->costFromStart + nn->estimatedCostToGoal;
			
		
			// Add to open list 
			if (nnIndex<0)
				{
				openNodes_.Add(nn);
				}
			else
				{
				openNodes_.Update(nnIndex);
				}
			
			}
	
		
		if (iterationSafetyLimit>0)
			{
			iterationCount--;
			}
		}
	
	Assert(iterationSafetyLimit==0 || iterationCount>0,"Iterator safety limit exceeded");
	
	// We didn't find a path, but we can build one to the closest cell we have
	BuildPath(bestNode,path);

	return false;
	}



//*** ClearLists ***

void PathFinder::ClearLists()
	{
	// Loop through the main node list and return all elements to the memory pool
	HashTableIterator<HashTableKey_Pointer,PathFinderNode*> it(nodes_);
	while (it.IsValid())
		{
		memoryPool_.Free(it.GetCurrent());
		it.MoveNext();
		}

	// Clear each of the lists
	nodes_.Clear(false);
	openNodes_.Clear(false);
	}



