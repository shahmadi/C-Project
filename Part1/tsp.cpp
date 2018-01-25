#include <iostream>
#include <algorithm>
#include "tsp.h"
using namespace std;


Graph::Graph(int V, int initial_vertex) 
{
	if(V < 1) 
	{
		cout << " number of vertexes <= 0\n";
		exit(1);
	}
	
	this->V = V; 
	this->initial_vertex = initial_vertex; 
	this->total_edges = 0;
	
}



void Graph::addEdge(int src, int dest, int weight) 
{
	map_edges[make_pair(src, dest)] = weight;
}


int Graph::existsEdge(int src, int dest) 
{
	map<pair<int, int>,int>::iterator it = map_edges.find(make_pair(src, dest));
	
	if(it != map_edges.end())
		return it->second;
	return -1;
}



Genetic::Genetic(Graph* graph, int size_population, int generations, int mutation_rate, bool show_population)
{
	if(size_population < 1) 
	{
		cout << " size_population < 1\n";
		exit(1);
	}
	else if(mutation_rate < 0 || mutation_rate > 100) 
	{
		cout << " mutation_rate  >= 0 and <= 100\n";
		exit(1);
	}
	this->graph = graph;
	this->size_population = size_population;
	this->real_size_population = 0;
	this->generations = generations;
	this->mutation_rate = mutation_rate;
	this->show_population = show_population;
}




int Genetic::isValidSolution(vector<int>& solution)
{
	int total_cost = 0;
	set<int> set_solution;
	
	
	for(int i = 0; i < graph->V; i++)
		set_solution.insert(solution[i]);
	
	if(set_solution.size() != (unsigned)graph->V)
		return -1;

	
	for(int i = 0; i < graph->V; i++)
	{
		if(i + 1 <  graph->V)
		{
			int cost = graph->existsEdge(solution[i], solution[i+1]);
			
			
			if(cost == -1)
				return -1;
			else
				total_cost += cost;
		}
		
	}
	return total_cost;
}


bool Genetic::existsChromosome(const vector<int> & v)
{
	
	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; 
		if(equal(v.begin(), v.end(), vec.begin())) 
			return true;
	}
	return false;
}


void Genetic::initialPopulation() 
{
	vector<int> parent;
	
	
	parent.push_back(graph->initial_vertex);
	
	
	for(int i = 0; i < graph->V; i++)
	{
		if(i != graph->initial_vertex)
			parent.push_back(i);
	}
		
	int total_cost = isValidSolution(parent);
	
	if(total_cost != -1) 
	{
		population.push_back(make_pair(parent, total_cost)); 
		real_size_population++; 
	}
	

	for(int i = 0; i < generations; i++)
	{
		
		random_shuffle(parent.begin() + 1, parent.begin() + (rand() % (graph->V - 1) + 1));
		
		int total_cost = isValidSolution(parent); 
		
		
		if(total_cost != -1 && !existsChromosome(parent))
		{
			population.push_back(make_pair(parent, total_cost));
			real_size_population++; 
		}
		if(real_size_population == size_population) 
			break;
	}
	
	
	if(real_size_population == 0)
		cout << "\nEmpty initial population";
	else
		sort(population.begin(), population.end(), sort_pred()); 
}


void Genetic::showPopulation()
{
	cout << "\nShowing solutions...\n\n";
	for(vector<pair<vector<int>, int> >::iterator it=population.begin(); it!=population.end(); ++it)
	{
		const vector<int>& vec = (*it).first; 
		
		for(int i = 0; i < graph->V; i++)
			cout << vec[i] << " ";
		cout << graph->initial_vertex;
		cout << "  Cost: " << (*it).second << "\n\n";
	}
	cout << "\n Population size: " << real_size_population << endl;
}

void Genetic::insertBinarySearch(vector<int>& child, int total_cost)
{
	int imin = 0;
	int imax = real_size_population - 1;
	
	while(imax >= imin)
	{
		int imid = imin + (imax - imin) / 2;
		
		if(total_cost == population[imid].second)
		{
			population.insert(population.begin() + imid, make_pair(child, total_cost));
			return;
		}
		else if(total_cost > population[imid].second)
			imin = imid + 1;
		else
			imax = imid - 1;
	}
	population.insert(population.begin() + imin, make_pair(child, total_cost));
}


void Genetic::crossOver(vector<int>& parent1, vector<int>& parent2)
{
	vector<int> child1, child2;
	

	map<int, int> genes1, genes2;
	
	for(int i = 0; i < graph->V; i++)
	{

		genes1[parent1[i]] = 0;
		genes2[parent2[i]] = 0;
	}
	

	int point1 = rand() % (graph->V - 1) + 1;
	int point2 = rand() % (graph->V - point1) + point1;
	
	
	if(point1 == point2)
	{
		if(point1 - 1 > 1)
			point1--;
		else if(point2 + 1 < graph->V)
			point2++;
		else
		{
			
			int point = rand() % 10 + 1; 
			if(point <= 5)
				point1--;
			else
				point2++;
		}
	}
	
	for(int i = 0; i < point1; i++)
	{

		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);

		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}
	

	for(int i = point2 + 1; i < graph->V; i++)
	{
		genes1[parent1[i]] = 1;
		genes2[parent2[i]] = 1;
	}
		

	for(int i = point2; i >= point1; i--)
	{
		if(genes1[parent2[i]] == 0) 
		{
			child1.push_back(parent2[i]);
			genes1[parent2[i]] = 1; 	
		}
		else
		{
			
			for(map<int, int>::iterator it = genes1.begin(); it != genes1.end(); ++it)
			{
				if(it->second == 0) 
				{
					child1.push_back(it->first);
					genes1[it->first] = 1; 
					break; 
				}
			}
		}
		
		if(genes2[parent1[i]] == 0) 
		{
			child2.push_back(parent1[i]);
			genes2[parent1[i]] = 1; 
		}
		else
		{
			
			for(map<int, int>::iterator it = genes2.begin(); it != genes2.end(); ++it)
			{
				if(it->second == 0) 
				{
					child2.push_back(it->first);
					genes2[it->first] = 1; 
					break; 
				}
			}
		}
	}
	
	
	for(int i = point2 + 1; i < graph->V; i++)
	{
		child1.push_back(parent1[i]);
		child2.push_back(parent2[i]);
	}
		

	int mutation = rand() % 100 + 1;
	if(mutation <= mutation_rate)
	{
	
		int index_gene1, index_gene2;
		index_gene1 = rand() % (graph->V - 1) + 1;
		index_gene2 = rand() % (graph->V - 1) + 1;
		

		int aux = child1[index_gene1];
		child1[index_gene1] = child1[index_gene2];
		child1[index_gene2] = aux;
		

		aux = child2[index_gene1];
		child2[index_gene1] = child2[index_gene2];
		child2[index_gene2] = aux;
	}
	
	int total_cost_child1 = isValidSolution(child1);
	int total_cost_child2 = isValidSolution(child2);
	

	if(total_cost_child1 != -1 && !existsChromosome(child1))
	{
		
		insertBinarySearch(child1, total_cost_child1); 
		real_size_population++; 
	}
	
	
	if(total_cost_child2 != -1 && !existsChromosome(child2))
	{
		
		insertBinarySearch(child2, total_cost_child2); 
		real_size_population++; 
	}
}


void Genetic::run()
{
	initialPopulation();
	
	if(real_size_population == 0)
		return;

	for(int i = 0; i < generations; i++)
	{
		int  old_size_population = real_size_population;
		
		
		if(real_size_population >= 2)
		{	
			if(real_size_population == 2)
			{
				
				crossOver(population[0].first, population[1].first);
			}
			else
			{
				
				
				int parent1, parent2;
			
				do
				{
					
					parent1 = rand() % real_size_population;
					parent2 = rand() % real_size_population;
				}while(parent1 == parent2);
				
				
				crossOver(population[parent1].first, population[parent2].first);
			}
			
			
			int diff_population = real_size_population - old_size_population;
			
			if(diff_population == 2)
			{
				if(real_size_population > size_population)
				{
				
					population.pop_back();
					population.pop_back();
					
					
					real_size_population -= 2;
				}
			}
			else if(diff_population == 1)
			{
				if(real_size_population > size_population)
				{
					population.pop_back(); 
					real_size_population--; 
				}
			}
		} 
		else 
		{
			
			crossOver(population[0].first, population[0].first);
			
			if(real_size_population > size_population)
			{
				population.pop_back(); 
				real_size_population--; 
			}
		}
	}
	
	if(show_population == true)
		showPopulation(); 
	
	cout << "\nBest solution: ";
	const vector<int>& vec = population[0].first;
	for(int i = 0; i < graph->V; i++)
		cout << vec[i] << " ";
	
	cout << "  Cost: " << population[0].second;
}


int Genetic::getCostBestSolution()
{
	if(real_size_population > 0)
		return population[0].second;
	return -1;
}
