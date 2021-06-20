/*
Given the head of a singly linked list, reverse the list, and return the reversed list.
Example 1:
Input: head = [1,2,3,4,5]
Output: [5,4,3,2,1]
Example 2:
Input: head = [1,2]
Output: [2,1]
Example 3:
Input: head = []
Output: []
Constraints:
The number of nodes in the list is the range [0, 5000].
-5000 <= Node.val <= 5000
Follow up: A linked list can be reversed either iteratively or recursively. Could you implement both?
*/

#include <iostream>
#include <unordered_map>
#include <string>

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        if(head->next == nullptr) {
            std::cout << "Return: " << head << " " << head->val << " Null" << std::endl;
            return head;
        }
        ListNode *temp = reverseList(head->next);
        std::cout << "Swap: " << head->next->next << " ";
        head->next->next = head; 
        std::cout << head->next->next << " " << head->next << std::endl;
        head->next = nullptr; 
        std::cout << "Return: " << temp << " " << temp->next << " " << temp->val << std::endl;
        return temp;
    }
};

int main() {
    Solution solution;
    [1,2,3,4,5]
    std::cout << solution.reverseList() << std::endl;
}