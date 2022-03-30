#pragma once

#include <iostream>

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class AddTwoNumbers {
public:
    AddTwoNumbers();
    virtual ~AddTwoNumbers();
private:
    ListNode* solution(ListNode* l1, ListNode* l2);
};