//#include "Graph.h"

//
//void AGraph::AddRelation(AActor* Edge, AActor* Connection)
//{
//	if (!IsEdgeInGraph(Edge))
//	{
//		TArray<AActor*> TConnections;
//		TConnections.Add(Connection);
//		FGNode* Temp = new FGNode{ false, Edge, TConnections };
//		Nodes.Add(Temp);
//	}
//	else 
//	{
//		FGNode* Temp = GetNodeByEdge(Edge);
//		Temp->Connections.AddUnique(Connection);
//	}
//
//}
//
//void AGraph::AddRelation(AActor* Edge, TArray<AActor*> Connections)
//{
//	if (!IsEdgeInGraph(Edge))
//	{
//		FGNode* Temp = new FGNode{ false, Edge, Connections };
//		Nodes.Add(Temp);
//	}
//	else
//	{
//		FGNode* Temp = GetNodeByEdge(Edge);
//		for (auto Connection : Temp->Connections)
//		{
//			Temp->Connections.AddUnique(Connection);
//		}
//	}
//}
//
//void AGraph::VisitEdge(AActor* Edge)
//{
//	for (auto Node : Nodes)
//	{
//		if (Node->Edge == Edge)
//			Node->bIsVisited = true;
//	}
//}
//
//bool AGraph::IsEdgeInGraph(AActor* Edge)
//{
//	for (auto Node : Nodes)
//	{
//		if (Node->Edge == Edge)
//		{
//			return true;
//		}
//	}
//	return false;
//}
//
//FGNode* AGraph::GetNodeByEdge(AActor* Edge)
//{
//	for (auto Node : Nodes)
//	{
//		if (Node->Edge == Edge)
//		{
//			return Node;
//		}
//	}
//	return nullptr;
//}
//
//TArray<AActor*> AGraph::BFS(AGraph G, AActor* Start)
//{
//	// Create a queue for BFS 
//	TArray<AActor*> Queue;
//
//	for (auto Relation : G.Nodes)
//	{
//		// Mark the current node as visited and enqueue it 
//		G.VisitEdge(Start);
//		Queue.Add(Start);
//
//		// 'i' will be used to get all adjacent 
//		// vertices of a vertex
//
//		for (auto CurrEdge : G.Nodes)
//		{
//			// Dequeue a vertex from queue and print it 
//			Start = Queue[0];
//			//cout << s << " ";
//			Queue.RemoveAt(0);
//
//			// Get all adjacent vertices of the dequeued 
//			// vertex s. If a adjacent has not been visited,  
//			// then mark it visited and enqueue it 
//			for (auto Node : G.GetRelations())
//			{
//				if (!Node->bIsVisited)
//				{
//					G.VisitEdge(Node->Edge);
//					Queue.Add(Node->Edge);
//				}
//			}
//		}
//	}
//
//	return Queue;
//}
//
//TArray< AActor*> AGraph::DFS(AGraph G, AActor* Start)
//{
//	// Create a queue for BFS 
//	TArray<AActor*> Queue;
//	return Queue;
//}
