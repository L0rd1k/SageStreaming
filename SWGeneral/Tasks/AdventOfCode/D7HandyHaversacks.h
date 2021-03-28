#pragma once

#include "FileHandler.h"
#include <iostream>
#include <regex>
#include <vector>
#include <cassert>
#include <iterator>

struct Node {
	int val;
	Node* next;
};

struct graphEdge {
	int start_ver, end_ver, weight;
};

class Graph {
public:
	Graph() {};
	virtual ~Graph() {};
	std::vector<Node*> vec_nodes;
	Node* insertNode(int value, Node* head) {
		Node* newNode = new Node;
		newNode->val = value;
		newNode->next = head;
		return newNode;
	}
};

class D7HandyHaversacks : public FileHandler {
public:
	D7HandyHaversacks();
	~D7HandyHaversacks();
private:
	int parseDataFile(std::string filePath) override;
private:
	std::ifstream file;
};

