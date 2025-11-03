// Reconstruct a binary tree from inorder and preorder traversals
// and output the postorder traversal sequence.

#include <stdio.h>
#include <string.h>

#define MAX_NODES 26

static char inorder[MAX_NODES + 1];
static char preorder[MAX_NODES + 1];
static char postorder[MAX_NODES + 1];
static int position[256];
static int post_index = 0;

static void build_postorder(int pre_left, int in_left, int length) {
    if (length <= 0) {
        return;
    }

    char root = preorder[pre_left];
    int root_index = position[(unsigned char)root];
    int left_length = root_index - in_left;
    int right_length = length - left_length - 1;

    build_postorder(pre_left + 1, in_left, left_length);
    build_postorder(pre_left + 1 + left_length, root_index + 1, right_length);

    postorder[post_index++] = root;
}

int main(void) {
    if (scanf("%26s%26s", inorder, preorder) != 2) {
        return 0;
    }

    int length = (int)strlen(inorder);

    for (int i = 0; i < length; ++i) {
        position[(unsigned char)inorder[i]] = i;
    }

    build_postorder(0, 0, length);
    postorder[post_index] = '\0';

    printf("%s\n", postorder);

    return 0;
}
