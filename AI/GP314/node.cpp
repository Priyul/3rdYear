#include "node.h"

Node::Node() {

}

double Node::evaluate(const vector<double>& features) {
    if (left_child == NULL && right_child == NULL) { //  leaf node
        return output;
    } else {
        if (features[feature_index] < threshold) { // left child
            return left_child->evaluate(features);
        } else { //  right child
            return right_child->evaluate(features);
        }
    }
}

