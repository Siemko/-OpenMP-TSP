#include <iostream>
#include <fstream>
#include <algorithm>
#include <conio.h>
#include <new>
#include <omp.h>
#include <vector>

void read_matrix(); //read from file
double cost(std::vector<int> perm); //calculate cost

//global vars:
double** matrix; //matrix from file
int node_num; //number of nodes from file
std::vector<int> perm_base;
std::vector<int> shortest;
double lowest_cost = INFINITY;


int main(int argc, char* argv[])
{
	read_matrix();
	for (int i = 0; i < node_num - 1; i++)
		perm_base.push_back(i + 1);
	_getch();
	int n = perm_base.size();
	double start = omp_get_wtime();
#pragma omp parallel for
	for (int i = 0; i < n; ++i)
	{
		// Make a copy of permutation_base
		auto perm = perm_base;
		// rotate the i'th  element to the front
		// keep the other elements sorted
		std::rotate(perm.begin(), perm.begin() + i, perm.begin() + i + 1);
		// Now go through all permutations of the last `n-1` elements. 
		// Keep the first element fixed. 
		do {
			cost(perm);
		} while (std::next_permutation(perm.begin() + 1, perm.end()));
	}
	double stop= omp_get_wtime();
	std::cout << lowest_cost << " : W czasie: " << (stop - start) * 1000 << "ms" << std::endl;
	for (int k = 0; k < shortest.size(); k++)
		std::cout << shortest[k] << " ->";
	std::cout << "0";
	_getch();
}
double cost(std::vector<int> perm)
{
	
	double c = 0;
	c += matrix[0][perm[0]];
	for (int i = 0; i < perm.size() - 1; i++)
	{
		c += matrix[perm[i]][perm[i + 1]];
	}
	c += matrix[0][perm[perm.size() - 1]];
	if (c < lowest_cost)
	{
		lowest_cost = c;
		shortest = perm;
	}
	return c;
}
void read_matrix()
{
	std::fstream file;
	file.open("matrix.txt", std::ios::in);
	if (file.good() == true)
	{
		file >> node_num;
		matrix = new double*[node_num];
		for (int i = 0; i < node_num; i++)
			matrix[i] = new double[node_num];

		for (int i = 0; i < node_num; i++)
		{
			for (int j = 0; j < node_num; j++)
			{
				file >> matrix[i][j];
			}
		}
		file.close();
	}
}