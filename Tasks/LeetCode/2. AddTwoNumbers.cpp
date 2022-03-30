#include "AddTwoNumbers.h"
    
    
AddTwoNumbers::AddTwoNumbers() {
    ListNode *f_node7 = new ListNode(9, nullptr);
    ListNode *f_node6 = new ListNode(9, f_node7);
    ListNode *f_node5 = new ListNode(9, f_node6);
    ListNode *f_node4 = new ListNode(9, f_node5);
    ListNode *f_node3 = new ListNode(9, f_node4);
    ListNode *f_node2 = new ListNode(9, f_node3);
    ListNode *f_node1 = new ListNode(9, f_node2);

    ListNode *s_node3 = new ListNode(8, nullptr);
    ListNode *s_node2 = new ListNode(8, s_node3);
    ListNode *s_node1 = new ListNode(8, s_node2);

    solution(f_node1, s_node1);

}
    
AddTwoNumbers::~AddTwoNumbers() {

}


ListNode* AddTwoNumbers::solution(ListNode* l1, ListNode* l2) {
    ListNode *result = new ListNode();             
    //while(l1 != nullptr || l2 != nullptr) { 
    //    std::cout << l1->val << " ";
    //        l1 = l1->next;
    //    } else {
    //        l1 = nullptr;
    //    }

    //    if(l2->next != nullptr) {
    //        std::cout << l2->val << " ";
    //        l2 = l2->next;
    //    } else {
    //        l2 = nullptr;
    //    }
    //    std::cout << std::endl;
    //}
    return new ListNode();
}
