#include "btree.h"
#include "test_util.h"
#include <stdio.h>
#include <stdlib.h>

const int base_data_count = 15;
const char base_keys[] = {'H', 'D', 'L', 'B', 'F', 'J', 'N', 'A',
                          'C', 'E', 'G', 'I', 'K', 'M', 'O'};
const int base_values[] = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 11, 13, 16};

const int additional_data_count = 6;
const char additional_keys[] = {'S', 'R', 'Q', 'P', 'X', 'Y', 'Z'};
const int additional_values[] = {10, 10, 10, 10, 10, 10};

const int rightmost_data_count = 7;
const char rightmost_keys[] = {'L', 'A', 'K', 'E', 'F', 'D', 'M'};
const int rightmost_values[] = {12, 1, 11, 5, 6, 4, 13};

const int traversal_data_count = 5;
const char traversal_keys[] = {'D', 'B', 'A', 'C', 'E'};
const int traversal_values[] = {1, 2, 3, 4, 5};

void red() {
  printf("\033[1;31m");
}

void green() {
  printf("\033[1;32m");
}

void cyan() {
  printf("\033[1;36m");
}

void reset_color(){
  printf("\033[0m");
}

int tests_passed = 0;
int exa_tests_passed = 0;
int tests_failed;
int exa_tests_failed;
int test_count = 0;

void init_test() {
  printf("Binary Search Tree - testing script\n");
  printf("-----------------------------------\n");
  printf("\n");
}

TEST(test_tree_init, "Initialize the tree")
bst_init(&test_tree);
ENDTEST

TEST(test_tree_dispose_empty, "Dispose the tree")
bst_init(&test_tree);
bst_dispose(&test_tree);
ENDTEST

TEST(test_tree_search_empty, "Search in an empty tree (A)")
test_count++;
bst_init(&test_tree);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'A', &result);
if (bool_res == true){
  red();
  printf("\nNode A was found! [TEST FAILED ☓]\n\n");
} else{
  green();
  printf("\nNode A was not found: [TEST PASSED ✓]\n\n");
  tests_passed++;
} 
reset_color();
ENDTEST

TEST(test_tree_insert_root, "Insert an item (H,1)")
test_count++;
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
bst_print_tree(test_tree);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
if (bool_res == true && result == 1){
  green();
  printf("Node H was inserted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node H was NOT inserted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
ENDTEST

TEST(test_tree_search_root, "Search in a single node tree (H)")
test_count++;
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
bst_print_tree(test_tree);
if (bool_res == true){
  green();
  printf("Node H was found correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node H was NOT found correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
ENDTEST

TEST(test_tree_update_root, "Update a node in a single node tree (H,1)->(H,8)")
test_count++;
bst_init(&test_tree);
bst_insert(&test_tree, 'H', 1);
bst_print_tree(test_tree);
bst_insert(&test_tree, 'H', 8);
bst_print_tree(test_tree);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
if (bool_res == true && result == 8){
  green();
  printf("Value of the H node was updated correctly! [TEST PASSED ✓]\n");
  printf("New value of the H node is: 8\n");
  tests_passed++;
} else {
  red();
  printf("Value of the H node was NOT updated correctly! [TEST PASSED ✓]\n\n");
}
reset_color();
ENDTEST

TEST(test_tree_insert_many, "Insert many values")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
cyan();
printf("\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("|  If you implemented the bst_insert() correctly, output below should look like this now  |\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("\n");
printf("Binary tree structure:\n");
printf("\n");
printf("           +-[O,16]\n");
printf("           |\n");
printf("        +-[N,14]\n");
printf("        |  |\n");
printf("        |  +-[M,13]\n");
printf("        |\n");
printf("     +-[L,12]\n");
printf("     |  |\n");
printf("     |  |  +-[K,11]\n");
printf("     |  |  |\n");
printf("     |  +-[J,10]\n");
printf("     |     |\n");
printf("     |     +-[I,9]\n");
printf("     |\n");
printf("  +-[H,8]\n");
printf("     |\n");
printf("     |     +-[G,7]\n");
printf("     |     |\n");
printf("     |  +-[F,6]\n");
printf("     |  |  |\n");
printf("     |  |  +-[E,5]\n");
printf("     |  |\n");
printf("     +-[D,4]\n");
printf("        |\n");
printf("        |  +-[C,3]\n");
printf("        |  |\n");
printf("        +-[B,2]\n");
printf("           |\n");
printf("           +-[A,1]\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search, "Search for an item deeper in the tree (A)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'A', &result);
if (bool_res == true){
  green();
  printf("\nNode A was found correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else{
  red();
  printf("\nNode A was NOT found correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_search_missing, "Search for a missing key (X)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'X', &result);
if (bool_res == true){
  red();
  printf("\nNode X was found INcorrectly: [TEST FAILED ☓]\n\n");
} else {
  green();
  printf("\nNode X was NOT found: [TEST PASSED ✓]\n\n");
  tests_passed++;
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_leaf, "Delete a leaf node (A)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'A');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'A', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node A was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node A was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_left_subtree, "Delete a node with only left subtree (R)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'R');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'R', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node R was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node R was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_right_subtree,
     "Delete a node with only right subtree (X)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);

bst_print_tree(test_tree);
bst_delete(&test_tree, 'X');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'X', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node X was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node X was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_both_subtrees, "Delete a node with both subtrees (L)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_insert_many(&test_tree, additional_keys, additional_values,
                additional_data_count);

bst_print_tree(test_tree);
bst_delete(&test_tree, 'L');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool_res = bst_search(test_tree, 'L', &result);
bool_res2 = bst_search(test_tree, 'H', &result2);
if (bool_res2 == true && bool_res == false){
  green();
  printf("Node L was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node L was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_rightmost_left_subtree, "Delete rightmost non-leaf node with left subtree (L)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, rightmost_keys, rightmost_values, rightmost_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'L');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool bool_res3;
bool bool_res4;
bool_res = bst_search(test_tree, 'L', &result);
bool_res2 = bst_search(test_tree, 'K', &result2);
bool_res3 = bst_search(test_tree, 'A', &result2);
bool_res4 = bst_search(test_tree, 'F', &result2);
if (bool_res4 == true && bool_res3 == true && bool_res2 == true && bool_res == false){
  green();
  printf("Node L was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node L was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_rightmost_first_node, "Delete righmost node right after target node with no children (E)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, rightmost_keys, rightmost_values, rightmost_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'E');
int result;
int result2;
bool bool_res;
bool bool_res2;
bool bool_res3;
bool bool_res4;
bool_res = bst_search(test_tree, 'E', &result);
bool_res2 = bst_search(test_tree, 'D', &result2);
bool_res3 = bst_search(test_tree, 'F', &result2);
bool_res4 = (test_tree->left->right->left->left == NULL);
if (bool_res4 == true && bool_res3 == true && bool_res2 == true && bool_res == false){
  green();
  printf("Node E was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
} else {
  red();
  printf("Node E was NOT deleted correctly: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_missing, "Delete a node that doesn't exist (U)")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'U');
cyan();
cyan();
printf("\n");
printf("--------------------------------------------------------------------------\n");
printf("|  Nothing should change ---> Output above and below should be the same  |\n");
printf("--------------------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_delete_root, "Delete the root node (H)")
test_count++;
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_delete(&test_tree, 'H');
int result;
bool bool_res;
bool_res = bst_search(test_tree, 'H', &result);
if (bool_res == true){
  red();
  printf("Node H was NOT deleted correctly: [TEST FAILED ☓]\n\n");
} else {
  green();
  printf("Node H was deleted correctly: [TEST PASSED ✓]\n\n");
  tests_passed++;
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_dispose_filled, "Dispose the whole tree")
bst_init(&test_tree);
bst_insert_many(&test_tree, base_keys, base_values, base_data_count);
bst_print_tree(test_tree);
bst_dispose(&test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------\n");
printf("|  Correct output below should be: Tree is empty  |\n");
printf("---------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_tree_preorder, "Traverse the tree using preorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_preorder(test_tree, test_items);
bst_print_tree(test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------------------\n");
printf("|  Correct output below should be: [D,1][B,2][A,3][C,4][E,5]  |\n");
printf("---------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_items(test_items);
ENDTEST

TEST(test_tree_inorder, "Traverse the tree using inorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_inorder(test_tree, test_items);
bst_print_tree(test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------------------\n");
printf("|  Correct output below should be: [A,3][B,2][C,4][D,1][E,5]  |\n");
printf("---------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_items(test_items);
ENDTEST

TEST(test_tree_postorder, "Traverse the tree using postorder")
bst_init(&test_tree);
bst_insert_many(&test_tree, traversal_keys, traversal_values, traversal_data_count);
bst_postorder(test_tree, test_items);
bst_print_tree(test_tree);
cyan();
printf("\n");
printf("---------------------------------------------------------------\n");
printf("|  Correct output below should be: [A,3][C,4][B,2][E,5][D,1]  |\n");
printf("---------------------------------------------------------------\n");
printf("\n");
reset_color();
bst_print_items(test_items);
ENDTEST

#ifdef EXA

TEST(test_letter_count_basic, "Count letters - Basic test");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCc_ 123 *");
int result, result2, result3, result4, result5;
bool bool_res, bool_res2, bool_res3, bool_res4, bool_res5;
bool_res = bst_search(test_tree, 'a', &result);
bool_res2 = bst_search(test_tree, 'b', &result2);
bool_res3 = bst_search(test_tree, 'c', &result3);
bool_res4 = bst_search(test_tree, '_', &result4);
bool_res5 = bst_search(test_tree, ' ', &result5);

if (bool_res == true && result == 1 &&
    bool_res2 == true && result2 == 2 &&
    bool_res3 == true && result3 == 3 &&
    bool_res4 == true && result4 == 5 &&
    bool_res5 == true && result5 == 2){
    green();
    printf("\nAll the nodes were inserted with the correct occurance count: [TEST PASSED ✓]\n\n");
    exa_tests_passed++;
      
} else {
  red();
  printf("There was an issue with inserting all the nodes and their values: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_balance_basic, "Count letters and balance - Basic test");
bst_init(&test_tree);
letter_count(&test_tree, "abBcCc_ 123 *");
bst_balance(&test_tree);
cyan();
printf("\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("|  If you implemented the bst_balance() correctly, output below should look like this now |\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("\n");
printf("Binary tree structure:\n");
printf("\n");
printf("        +-[c,3]\n");
printf("        |\n");
printf("     +-[b,2]\n");
printf("     |\n");
printf("  +-[a,1]\n");
printf("     |\n");
printf("     |  +-[_,5]\n");
printf("     |  |\n");
printf("     +-[ ,2]\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_letter_count_advanced, "Count letters - Advanced test");
bst_init(&test_tree);
letter_count(&test_tree, "CdEfGhAb 12 *)=+-3 ");
int result, result2, result3, result4, result5, result6, result7, result8, result9, result10;
bool bool_res, bool_res2, bool_res3, bool_res4, bool_res5, bool_res6, bool_res7, bool_res8, bool_res9, bool_res10;
bool_res = bst_search(test_tree, 'a', &result);
bool_res2 = bst_search(test_tree, 'b', &result2);
bool_res3 = bst_search(test_tree, 'c', &result3);
bool_res4 = bst_search(test_tree, 'd', &result4);
bool_res5 = bst_search(test_tree, 'e', &result5);
bool_res6 = bst_search(test_tree, 'f', &result6);
bool_res7 = bst_search(test_tree, 'g', &result7);
bool_res8 = bst_search(test_tree, 'h', &result8);
bool_res9 = bst_search(test_tree, '_', &result9);
bool_res10 = bst_search(test_tree, ' ', &result10);
if (bool_res == true && result == 1 &&
    bool_res2 == true && result2 == 1 &&
    bool_res3 == true && result3 == 1 &&
    bool_res4 == true && result4 == 1 &&
    bool_res5 == true && result5 == 1 && 
    bool_res6 == true && result6 == 1 &&
    bool_res7 == true && result7 == 1 &&
    bool_res8 == true && result8 == 1 &&
    bool_res9 == true && result9 == 8 &&
    bool_res10 == true && result10 == 3
    ){
    green();
    printf("\nAll the nodes were inserted with the correct occurance count: [TEST PASSED ✓]\n\n");
    exa_tests_passed++;
      
} else {
  red();
  printf("There was an issue with inserting all the nodes and their values: [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_balance_advanced, "Count letters and balance - advanced test");
bst_init(&test_tree);
letter_count(&test_tree, "CdEfGh 12 3 *)=+-ab");
bst_balance(&test_tree);
cyan();
printf("\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("|  If you implemented the bst_balance() correctly, output below should look like this now |\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("\n");
printf("Binary tree structure:\n");
printf("\n");
printf("           +-[h,1]\n");
printf("           |\n");
printf("        +-[g,1]\n");
printf("        |\n");
printf("     +-[f,1]\n");
printf("     |  |\n");
printf("     |  |  +-[e,1]\n");
printf("     |  |  |\n");
printf("     |  +-[d,1]\n");
printf("     |\n");
printf("  +-[c,1]\n");
printf("     |\n");
printf("     |     +-[b,1]\n");
printf("     |     |\n");
printf("     |  +-[a,1]\n");
printf("     |  |\n");
printf("     +-[_,8]\n");
printf("        |\n");
printf("        +-[ ,3]\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_letter_count_only_letters, "Count letters - only letters");
bst_init(&test_tree);
letter_count(&test_tree, "aBbBcDDDDeFGHIIIiiJKLmnOOOpQQrsTuuUvWWxXxXyZZZZZ");
int result, result2, result3, result4, result5, result6, result7, result8, result9, result10, result11, result12, result13, result14, result15, result16, result17, result18, result19, result20, result21, result22, result23, result24, result25, result26;
bool bool_res, bool_res2, bool_res3, bool_res4, bool_res5, bool_res6, bool_res7, bool_res8, bool_res9, bool_res10, bool_res11, bool_res12, bool_res13, bool_res14, bool_res15, bool_res16, bool_res17, bool_res18, bool_res19, bool_res20, bool_res21, bool_res22, bool_res23, bool_res24, bool_res25, bool_res26;
bool_res = bst_search(test_tree, 'a', &result);
bool_res2 = bst_search(test_tree, 'b', &result2);
bool_res3 = bst_search(test_tree, 'c', &result3);
bool_res4 = bst_search(test_tree, 'd', &result4);
bool_res5 = bst_search(test_tree, 'e', &result5);
bool_res6 = bst_search(test_tree, 'f', &result6);
bool_res7 = bst_search(test_tree, 'g', &result7);
bool_res8 = bst_search(test_tree, 'h', &result8);
bool_res9 = bst_search(test_tree, 'i', &result9);
bool_res10 = bst_search(test_tree, 'j', &result10);
bool_res11 = bst_search(test_tree, 'k', &result11);
bool_res12 = bst_search(test_tree, 'l', &result12);
bool_res13 = bst_search(test_tree, 'm', &result13);
bool_res14 = bst_search(test_tree, 'n', &result14);
bool_res15 = bst_search(test_tree, 'o', &result15);
bool_res16 = bst_search(test_tree, 'p', &result16);
bool_res17 = bst_search(test_tree, 'q', &result17);
bool_res18 = bst_search(test_tree, 'r', &result18);
bool_res19 = bst_search(test_tree, 's', &result19);
bool_res20 = bst_search(test_tree, 't', &result20);
bool_res21 = bst_search(test_tree, 'u', &result21);
bool_res22 = bst_search(test_tree, 'v', &result22);
bool_res23 = bst_search(test_tree, 'w', &result23);
bool_res24 = bst_search(test_tree, 'x', &result24);
bool_res25 = bst_search(test_tree, 'y', &result25);
bool_res26 = bst_search(test_tree, 'z', &result26);
if (bool_res == true && result == 1 &&
    bool_res2 == true && result2 == 3 &&
    bool_res3 == true && result3 == 1 &&
    bool_res4 == true && result4 == 4 &&
    bool_res5 == true && result5 == 1 && 
    bool_res6 == true && result6 == 1 &&
    bool_res7 == true && result7 == 1 &&
    bool_res8 == true && result8 == 1 &&
    bool_res9 == true && result9 == 5 &&
    bool_res10 == true && result10 == 1 &&
    bool_res11 == true && result11 == 1 &&
    bool_res12 == true && result12 == 1 &&
    bool_res13 == true && result13 == 1 &&
    bool_res14 == true && result14 == 1 &&
    bool_res15 == true && result15 == 3 &&
    bool_res16 == true && result16 == 1 &&
    bool_res17 == true && result17 == 2 &&
    bool_res18 == true && result18 == 1 &&
    bool_res19 == true && result19 == 1 &&
    bool_res20 == true && result20 == 1 &&
    bool_res21 == true && result21 == 3 &&
    bool_res22 == true && result22 == 1 &&
    bool_res23 == true && result23 == 2 &&
    bool_res24 == true && result24 == 4 &&
    bool_res25 == true && result25 == 1 &&
    bool_res26 == true && result26 == 5
    ){
    green();
    printf("\nAll the nodes were inserted with the correct occurance count: [TEST PASSED ✓]\n\n");
    exa_tests_passed++;
      
} else {
  red();
  printf("There was an issue with inserting all the nodes and their values : [TEST FAILED ☓]\n\n");
}
reset_color();
bst_print_tree(test_tree);
ENDTEST

TEST(test_balance_only_letters, "Count letters and balance - only letters");
bst_init(&test_tree);
letter_count(&test_tree, "aBbBcDDDDeFGHIIIiiJKLmnOOOpQQrsTuuUvWWxXxXyZZZZZ");
bst_balance(&test_tree);
cyan();
printf("\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("|  If you implemented the bst_balance() correctly, output below should look like this now |\n");
printf("-------------------------------------------------------------------------------------------\n");
printf("\n");
printf("Binary tree structure:\n");
printf("\n");
printf("              +-[z,5]\n");
printf("              |\n");
printf("           +-[y,1]\n");
printf("           |  |\n");
printf("           |  +-[x,4]\n");
printf("           |\n");
printf("        +-[w,2]\n");
printf("        |  |\n");
printf("        |  |  +-[v,1]\n");
printf("        |  |  |\n");
printf("        |  +-[u,3]\n");
printf("        |\n");
printf("     +-[t,1]\n");
printf("     |  |\n");
printf("     |  |     +-[s,1]\n");
printf("     |  |     |\n");
printf("     |  |  +-[r,1]\n");
printf("     |  |  |  |\n");
printf("     |  |  |  +-[q,2]\n");
printf("     |  |  |\n");
printf("     |  +-[p,1]\n");
printf("     |     |\n");
printf("     |     |  +-[o,3]\n");
printf("     |     |  |\n");
printf("     |     +-[n,1]\n");
printf("     |\n");
printf("  +-[m,1]\n");
printf("     |\n");
printf("     |        +-[l,1]\n");
printf("     |        |\n");
printf("     |     +-[k,1]\n");
printf("     |     |  |\n");
printf("     |     |  +-[j,1]\n");
printf("     |  +-[i,5]\n");
printf("     |  |  |\n");
printf("     |  |  |  +-[h,1]\n");
printf("     |  |  |  |\n");
printf("     |  |  +-[g,1]\n");
printf("     |  |\n");
printf("     +-[f,1]\n");
printf("        |\n");
printf("        |     +-[e,1]\n");
printf("        |     |\n");
printf("        |  +-[d,4]\n");
printf("        |  |\n");
printf("        +-[c,1]\n");
printf("           |\n");
printf("           |  +-[b,3]\n");
printf("           |  |\n");
printf("           +-[a,1]\n");
printf("\n");
reset_color();
bst_print_tree(test_tree);
ENDTEST

void print_exa_tests(){
  exa_tests_failed = 3 - exa_tests_passed;
  printf("\n");
  printf("---------- EXA TESTS SUMMARY ----------\n");
  printf("|                                     |\n");
  printf("|");
  green();
  printf(" EXA TESTS PASSED: %d", exa_tests_passed);
  reset_color();
  printf(" ");
  printf("                |\n");
  printf("|");
  red();
  printf(" EXA TESTS FAILED: %d", exa_tests_failed);
  reset_color();
  printf(" ");
  printf("                |\n");
  printf("|                                     |\n");
  printf("---------------------------------------\n");
  printf("\n");
  reset_color();
}
#endif // EXA

int main(int argc, char *argv[]) {
  init_test();

  test_tree_init();
  test_tree_dispose_empty();
  test_tree_search_empty();
  test_tree_insert_root();
  test_tree_search_root();
  test_tree_update_root();
  test_tree_insert_many();
  test_tree_search();
  test_tree_search_missing();
  test_tree_delete_leaf();
  test_tree_delete_left_subtree();
  test_tree_delete_right_subtree();
  test_tree_delete_both_subtrees();
  test_tree_delete_rightmost_left_subtree();
  test_tree_delete_rightmost_first_node();
  test_tree_delete_missing();
  test_tree_delete_root();
  test_tree_dispose_filled();
  test_tree_preorder();
  test_tree_inorder();
  test_tree_postorder();
  
  tests_failed = test_count - tests_passed;
  printf("\n");
  printf("---------- TESTS SUMMARY ----------\n");
  printf("|                                 |\n");
  printf("|");
  green();
  printf(" TESTS PASSED: %d", tests_passed);
  reset_color();
  if(tests_passed < 10)
    printf(" ");
  printf("                |\n");
  printf("|");
  red();
  printf(" TESTS FAILED: %d", tests_failed);
  reset_color();
    if(tests_failed < 10)
    printf(" ");
  printf("                |\n");
  printf("|                                 |\n");
  printf("-----------------------------------\n");
  printf("\n");
  reset_color();

#ifdef EXA
  test_letter_count_basic();
  test_balance_basic();
  test_letter_count_advanced();
  test_balance_advanced();
  test_letter_count_only_letters();
  test_balance_only_letters();
  print_exa_tests();
#endif // EXA
}
/* author ~ xcuprm01 (+ desktop13)*/
