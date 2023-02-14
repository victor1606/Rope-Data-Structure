#include "./rope.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define EMPTY ""

char *strdup(const char *string);

RopeNode* makeRopeNode(const char* str) {
	RopeNode *node;
	// alocare memorie
	node = malloc(sizeof(RopeNode));
	if (node == NULL) {
		return NULL;
	}

	// atribuire valori
	node->str = str;
	node->left = NULL;
	node->right = NULL;
	node->weight = strlen(str);
	return node;
}

RopeTree* makeRopeTree(RopeNode* root) {
	RopeTree *tree;
	// alocare memorie
	tree = malloc(sizeof(RopeTree));
	if (tree == NULL) {
		return NULL;
	}

	tree->root = root;

	return tree;
}

void printRopeNode(RopeNode* rn) {
	if (!rn)
		return;
	if (!(rn->left) && !(rn->right)) {
		printf("%s", rn->str);
		return;
	}

	printRopeNode(rn->left);
	printRopeNode(rn->right);
}

void printRopeTree(RopeTree* rt) {
	if (rt && rt->root) {
		printRopeNode(rt->root);
	}
	printf("\n");
}

void debugRopeNode(RopeNode* rn, int indent) {
	if (!rn)
		return;

	for (int i = 0; i < indent; ++i)
		printf("%s", " ");

	if (!strcmp(rn->str, EMPTY))
		printf("# %d\n", rn->weight);
	else
		printf("%s %d\n", rn->str, rn->weight);

	debugRopeNode(rn->left, indent+2);
	debugRopeNode(rn->right, indent+2);
}

int getTotalWeight(RopeNode* rt) {
	if (!rt)
		return 0;

	return rt->weight + getTotalWeight(rt->right);
}

// returneaza greuatea nodului rt
int getWeight(RopeNode* rt) {
	if (!rt)
		return 0;

	return rt->weight + getWeight(rt->left);
}

// returneaza RopeTree-ul format prin alipirea lui rt1 si rt2
RopeTree* concat(RopeTree* rt1, RopeTree* rt2) {
    // formarea nodului-radacina a noului arbore
	char *aux = malloc(2 * sizeof(char));
	strcpy(aux, EMPTY);
	RopeNode *root = makeRopeNode(aux);

	// formarea ramurilor noului arbore
	root->left = rt1->root;
	root->right = rt2->root;
	root->weight = getWeight(rt1->root);

	// formarea arborelui
	RopeTree *result = makeRopeTree(root);
	return result;
}

// functia cauta recursiv caracterul de pe pozitia idx
// step reprezinta indicele la care a ajuns cautarea,
// ok == 1 -> caracter gasit; ok == 0 -> caz contrar
char find_index(RopeNode *root, int idx, int *step, int *ok) {
	char c;
	if (root != NULL && root->left != NULL && (*ok) == 0) {
	   c = find_index(root->left, idx, step, ok);
	}

	for (unsigned int i = 0; i < strlen(root->str) && (*ok) == 0; i++) {
		if (idx == *step) {
			(*ok) = 1;
			return (char)root->str[i];
		}
		(*step)++;
	}

	if (root != NULL && root->right != NULL && (*ok) == 0) {
		c = find_index(root->right, idx, step, ok);
	}

	return  c;
}

// returneaza caracterul de pe pozitia idx
char indexRope(RopeTree* rt, int idx) {
	int step = 0, ok = 0;
	return find_index(rt->root, idx, &step, &ok);
}

// returneza subsirul cuprins intre indicele
// start si intre indicele end
char* search(RopeTree* rt, int start, int end) {
	char *result;
	result = calloc(1000, sizeof(char));
	int contor = 0;
	for (int i = start; i < end; i++) {
		result[contor] = indexRope(rt, i);
		contor++;
	}
	return result;
}

// cauta recursiv si returneaza nodul care contine
// aracterul din sir de pe pozitia idx
// step reprezinta indicele la care a ajuns cautarea,
// ok == 1 -> caracter gasit; ok == 0 -> caz contrar
RopeNode *find_node(RopeNode *root, int *idx, int *step, int *ok) {
	RopeNode *aux;
	if (root == NULL) {
		return  NULL;
	}

	if (root != NULL && root->left != NULL && (*ok) == 0) {
		aux = find_node(root->left, idx, step, ok);
	}

	for (unsigned int i = 0; i < strlen(root->str) && (*ok) == 0; i++) {
		if ((*idx) == (*step)) {
			(*ok) = 1;
			(*idx) = i;
			return root;
		}
		(*step)++;
	}

	if (root != NULL && root->right != NULL && (*ok) == 0) {
		aux = find_node(root->right, idx, step, ok);
	}

	return  aux;
}


// cauta recursiv si returneaza parintele nodului child
// ok == 1 -> parinte gasit; ok == 0 -> caz contrar
// side are valoare 1 daca child e copil drept si 0 daca e copil stang
RopeNode *find_parent(RopeNode *root, RopeNode *child, int *side, int *ok) {
	if (root == NULL || child == NULL) {
		return NULL;
	}
	RopeNode *parent;
	if (root->left == child) {
		(*side) = 0;
		(*ok) = 1;
		return root;
	}
	if (root->right == child) {
		(*side) = 1;
		(*ok) = 1;
		return root;
	}
	if ((*ok) == 0) {
		parent = find_parent(root->left, child, side, ok);
	}
	if ((*ok) == 0) {
		parent = find_parent(root->right, child, side, ok);
	}

	return parent;
}

// imparte sirul in functie de caracterul
// de pe pozitia idx
SplitPair split(RopeTree* rt, int idx) {
	RopeNode *node, *node1, *node2;
	RopeNode *parent;
	RopeTree *tr1, *tr2, *tr_left, *tr_aux;
	SplitPair result;
	char string1[10000], string2[10000];
	int step = 0, ok = 0, free_tr_left = 0;
	int side;
	node = find_node(rt->root, &idx, &step, &ok);

	// cazul in care split-ul de face dupa ultimul caracter din sir
	if (ok == 0) {
		node1 = makeRopeNode(strdup(EMPTY));
		tr1 = makeRopeTree(node1);
		tr_left = rt;
	} else {
		int length = strlen(node->str);
		// nodul care contine caracterul cautat
		// contine un sir de caractere
		if (length > 1) {
			int k = 0, i = 0;
			// impartirea sirului de caractere din node
			// in functie de idx
			if (idx == 0) {
				string1[i] = '\0';
			} else {
				while (k < idx) {
					string1[i++] = node->str[k++];
				}
				string1[i] = '\0';
			}
			i = 0;
			if (idx == length) {
				string2[i] = '\0';
			} else {
				while (k <= length) {
					string2[i++] = node->str[k++];
				}
			}

			// formarea nodurilor-radacina, respectiv a
			// subarborilor in care se imparte arborele initial
			node1 = makeRopeNode(strdup(string1));
			tr_left = makeRopeTree(node1);
			free_tr_left = 1;

			node2 = makeRopeNode(strdup(string2));
			tr1 = makeRopeTree(node2);

			ok = 0;
			parent = find_parent(rt->root, node, &side, &ok);
		} else {
		    // cazul in care nodul contine doar
		    // caracterul cautat
			node1 = node;
			tr1 = makeRopeTree(node1);
			ok = 0;
			parent = find_parent(rt->root, node1, &side, &ok);
			if (side == 1) {
                tr_left = makeRopeTree(parent->left);
			}
		}
		// verificare daca parintele lui node mai are si alti copii
		// si concatenarea acestora cu subarborele corespunzator
		if (parent != rt->root) {
			if (side == 0 && parent->right != NULL) {
				tr2 = makeRopeTree(parent->right);
				RopeTree *aux = tr1;
				tr1 = concat(tr1, tr2);
				free(aux);
				free(tr2);
			} else {
				if (side == 1 && parent->left != NULL) {
					tr_aux = makeRopeTree(parent->left);
					RopeTree *aux = tr_left;
					tr_left = concat(tr_aux, tr_left);
					free(aux);
					free(tr_aux);
					free_tr_left = 1;
				}
			}
		}
		// verificarea tuturor parintilor pana la radacina si
		// alipirea celorlalti copii la subarborii corespuzatori
		while (parent != rt->root) {
			ok = 0;
			parent = find_parent(rt->root, parent, &side, &ok);
			if (parent != NULL && parent->right != NULL && side == 0) {
				tr2 = makeRopeTree(parent->right);
				RopeTree *aux = tr1;
				tr1 = concat(tr1, tr2);
				free(aux);
				free(tr2);
			}
			if (parent != NULL && parent->left != NULL && side == 1) {
				tr_aux = makeRopeTree(parent->left);
				free_tr_left = 1;
				RopeTree *aux = tr_left;
				tr_left = concat(tr_aux, tr_left);
				free(aux);
				free(tr_aux);
			}
		}
	}

	result.right = tr1->root;
	result.left = tr_left->root;
	free(tr1);

	if (free_tr_left == 1)
		free(tr_left);

	return result;
}

// insereaza sirul str incepand cu pozitia idx
RopeTree* insert(RopeTree* rt, int idx, const char* str) {
    // imparte sirul in functie de idx
	SplitPair sp = split(rt, idx);

	// formarea subarborilor corespunzatori
	RopeTree* rt1 = makeRopeTree(sp.left);
	RopeTree* rt2 = makeRopeTree(sp.right);

	// crearea nodului care se doreste a se adauga
	// si a arborelui corespunzator
	RopeNode *new_node = makeRopeNode(str);
	RopeTree *new_tree = makeRopeTree(new_node);

	// alipirea partilor initiale ale arborelui
	RopeTree *result = concat(rt1, new_tree);
	RopeTree *aux = result;
	result = concat(result, rt2);
	// eliberare memorie auxiliara folosita
	free(aux);
	free(new_tree);
	free(rt1);
	free(rt2);

	return result;
}

// sterge sirul cuprins intre start si start + len
RopeTree* delete(RopeTree* rt, int start, int len) {
    // imparte sirul in functie de indicele start
    // si creeaza subarborele corespunzator
	SplitPair sp_1 = split(rt, start);
	RopeTree* rt1 = makeRopeTree(sp_1.left);

	// imparte sirul initial in functie de start + len
	// si creeaza arborele corespunzator
	SplitPair sp_2 = split(rt, start + len);
	RopeTree* rt2 = makeRopeTree(sp_2.right);
	// alipeste noile parti ale arborelui
	RopeTree *result = concat(rt1, rt2);

	// elibereaza memoria auxiliara folosita
	free(rt1);
	free(rt2);

	return result;
}

// FINAL 10p -> complex test involving all operations
