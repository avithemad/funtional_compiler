#include <iostream>
#include <map>
#include <vector>
#include <string>

#define NEWNODE (struct ast_node *)malloc(sizeof(struct ast_node));

struct ast_node
{
	int node_type;
	int value;
	int identifier_idx;
	// max child now is 3, for lamda expr for others it is one
	struct ast_node *children[3];
};

enum Token
{
	tok_equal = -16,
	tok_else = -15,
	tok_then = -14,
	tok_if = -13,
	tok_op = -12,
	tok_comma = -11,
	tok_apply = -10,
	tok_variable = -9,
	tok_lamda = -8,
	tok_boolean = -7,
	tok_integer = -6,
	tok_l_paren = -5,
	tok_r_paren = -4,
	tok_l_curl = -3,
	tok_r_curl = -2,
	tok_eof = -1
};

static std::string Identifier;
static int value;
std::vector<int> token_stream;
std::map<int, int> value_map;
std::map<int, std::string> id_map;
static int current_token_idx = 0;

// Parser Implementation

static int parsing_idx = 0;

enum AstNodeType
{
	node_ifthenelse,
	node_op,
	node_lamda,
	node_variable,
	node_value,
	node_integer,
	node_boolean,
	node_app
};

struct ast_node *parse_value();
struct ast_node *parse_term();
struct ast_node *parse_variable()
{
	int current_index = parsing_idx;

	if (token_stream[parsing_idx++] != tok_variable)
	{
		// Handle error
		return NULL;
	};

	struct ast_node *var_node = (ast_node *)malloc(sizeof(ast_node));
	var_node->node_type = node_variable;
	var_node->identifier_idx = current_index;
	// var_node->identifier ="dsf";

	return var_node;
}

struct ast_node *parse_lamda()
{
	if (token_stream[parsing_idx++] != tok_lamda)
		return NULL;
	if (token_stream[parsing_idx++] != tok_l_paren)
	{
		return NULL;
	}
	struct ast_node *leaf_var_node = parse_variable();
	if (leaf_var_node == NULL)
	{
		return NULL;
	}
	if (token_stream[parsing_idx++] != tok_r_paren)
	{
		return NULL;
	}
	if (token_stream[parsing_idx++] != tok_l_curl)
	{
		return NULL;
	}
	struct ast_node *inner_value = parse_term();
	if (inner_value == NULL)
		return NULL;

	if (token_stream[parsing_idx++] != tok_r_curl)
	{
		return NULL;
	}

	struct ast_node *lamda_node = (struct ast_node *)malloc(sizeof(struct ast_node));
	lamda_node->node_type = node_lamda;

	lamda_node->children[0] = leaf_var_node;
	lamda_node->children[1] = inner_value;

	return lamda_node;
}

struct ast_node *parse_integer()
{
	if (token_stream[parsing_idx++] == tok_integer)
	{
		struct ast_node *int_node = (struct ast_node *)malloc(sizeof(struct ast_node));
		int_node->node_type = node_integer;
		int_node->value = value_map[parsing_idx - 1];

		return int_node;
	}

	return NULL;
}

struct ast_node *parse_value()
{
	int current_index = parsing_idx;

	if (token_stream[current_index] == tok_lamda)
	{
		struct ast_node *lamda_node = parse_lamda();
		if (lamda_node == NULL)
			return NULL;

		return lamda_node;
	}

	if (token_stream[current_index] == tok_integer)
	{
		struct ast_node *int_node = parse_integer();
		if (int_node == NULL)
			return NULL;

		return int_node;
	}

	return NULL;
}

struct ast_node *parse_app()
{
	if (parsing_idx >= token_stream.size())
		return NULL;
	int current_index = parsing_idx;

	if (token_stream[parsing_idx++] != tok_apply)
		return NULL;
	if (token_stream[parsing_idx++] != tok_l_paren)
		return NULL;
	struct ast_node *t1 = parse_term();

	if (token_stream[parsing_idx++] != tok_comma)
	{
		return NULL;
	}
	struct ast_node *t2 = parse_term();
	if (token_stream[parsing_idx++] != tok_r_paren)
	{
		return NULL;
	}
	struct ast_node *app_node = NEWNODE;
	app_node->node_type = node_app;
	app_node->children[0] = t1;
	app_node->children[1] = t2;
	return app_node;
}

struct ast_node *parse_boolean()
{
	if (token_stream[parsing_idx++] == tok_boolean)
	{
		struct ast_node *bool_node = NEWNODE;
		bool_node->node_type = node_boolean;
		bool_node->identifier_idx = parsing_idx - 1;

		return bool_node;
	}

	return NULL;
}

struct ast_node *parse_operation()
{
	int current_index = parsing_idx;
	if (token_stream[parsing_idx++] == tok_op)
	{
		if (token_stream[parsing_idx++] != tok_l_paren)
			return NULL;
		struct ast_node *t1 = parse_term();
		if (token_stream[parsing_idx++] != tok_comma)
			return NULL;
		struct ast_node *t2 = parse_term();
			if (token_stream[parsing_idx++] != tok_r_paren)
		return NULL;
		struct ast_node *op_node = new ast_node;
		op_node->children[0] = t1;
		op_node->children[1] = t2;
		op_node->node_type = node_op;
		op_node->identifier_idx = current_index;

		return op_node;
	}
	return NULL;
}

struct ast_node* parse_ifthenelse() {
	if (token_stream[parsing_idx++] == tok_if) {
		if (token_stream[parsing_idx++]!=tok_l_paren) return NULL;
		struct ast_node* t1 = parse_term();
		if (token_stream[parsing_idx++]!=tok_r_paren) return NULL;
		if (token_stream[parsing_idx++]!=tok_then) return NULL;
		if (token_stream[parsing_idx++]!=tok_l_curl) return NULL;
		struct ast_node* t2 = parse_term();
		if (token_stream[parsing_idx++]!=tok_r_curl) return NULL;
		if (token_stream[parsing_idx++]!=tok_else) return NULL;
		if (token_stream[parsing_idx++]!=tok_l_curl) return NULL;
		struct ast_node* t3 = parse_term();
		if (token_stream[parsing_idx++]!=tok_r_curl) return NULL;

		struct ast_node* if_node = new ast_node;
		if_node->children[0] = t1;
		if_node->children[1] = t2;
		if_node->children[2] = t3;
		if_node->node_type = node_ifthenelse;
		
		return if_node;
	}
	return NULL;
}

struct ast_node *parse_term()
{
	int current_index = parsing_idx;

	if (token_stream[current_index] == tok_variable)
	{
		// if you see a variable, either grammer can happen
		// t op t, just a variable
		return parse_variable();
	}
	if (token_stream[current_index] == tok_lamda)
	{
		return parse_lamda();
	}
	if (token_stream[current_index] == tok_apply)
	{
		return parse_app();
	}
	if (token_stream[current_index] == tok_integer)
	{
		return parse_integer();
	}
	if (token_stream[current_index] == tok_boolean)
	{
		return parse_boolean();
	}
	// TODO: parse operational and ifthenelse...
	if (token_stream[current_index] == tok_op)
	{
		return parse_operation();
	}
	if (token_stream[current_index] == tok_if) {
		return parse_ifthenelse();
	}
	return NULL;
}

// Lexer Implementation

bool is_space(int ch)
{
	return ch == ' ' || ch == '\n' || ch == '\t';
}

bool is_alpha(int ch)
{
	return (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A');
}

bool is_num(int ch)
{
	return (ch <= '9' && ch >= '0');
}

int get_token()
{
	static int last_char = ' ';
	while (is_space(last_char))
		last_char = getchar();
	if (last_char == ',')
	{
		last_char = getchar();
		return tok_comma;
	}
	if (last_char == '+')
	{
		last_char = getchar();
		id_map[current_token_idx] = '+';
		return tok_op;
	}
	if (last_char == '-')
	{
		last_char = getchar();
		id_map[current_token_idx] = '-';
		return tok_op;
	}
	if (last_char == '/')
	{
		last_char = getchar();
		id_map[current_token_idx] = '/';
		return tok_op;
	}
	if (last_char == '*')
	{
		last_char = getchar();
		id_map[current_token_idx] = '*';
		return tok_op;
	}
	if (last_char == '=')
	{
		last_char = getchar();
		id_map[current_token_idx] = '=';
		return tok_op;
	}
	if (is_alpha(last_char))
	{
		Identifier = last_char;
		while (is_alpha(last_char = getchar()))
		{
			Identifier += last_char;
		}
		id_map[current_token_idx] = Identifier;
		if (Identifier == "lamda")
		{
			return tok_lamda;
		}

		if (Identifier == "true" || Identifier == "false")
		{
			return tok_boolean;
		}

		if (Identifier == "apply")
		{
			return tok_apply;
		}

		if (Identifier == "if") {
			return tok_if;
		}
		if (Identifier == "then") {
			return tok_then;
		}
		if (Identifier == "else") {
			return tok_else;
		}
		return tok_variable;
	}

	if (is_num(last_char))
	{
		std::string number;
		number += last_char;
		while (is_num(last_char = getchar()))
		{
			number += last_char;
		}
		value = strtod(number.c_str(), 0);
		value_map[current_token_idx] = value;
		return tok_integer;
	}

	if (last_char == '(')
	{
		last_char = getchar();
		return tok_l_paren;
	}
	if (last_char == ')')
	{
		last_char = getchar();
		return tok_r_paren;
	}

	if (last_char == '{')
	{
		last_char = getchar();
		return tok_l_curl;
	}

	if (last_char == '}')
	{
		last_char = getchar();
		return tok_r_curl;
	}

	if (last_char == EOF)
	{
		return tok_eof;
	}

	last_char = getchar();
	return 0;
}

void print_node(struct ast_node *node)
{
	if (node->node_type == node_variable)
	{
		std::cout << "var_node_" << id_map[node->identifier_idx];
	}
	else if (node->node_type == node_lamda)
	{
		std::cout << "lamda_node";
	}
	else if (node->node_type == node_integer)
	{
		std::cout << "int_node_" << node->value;
	}
	else if (node->node_type == node_app)
	{
		std::cout << "app_node";
	}
	else if (node->node_type == node_boolean)
	{
		std::cout << "bool_node_" << id_map[node->identifier_idx];
	}
	else if (node->node_type == node_op) {
		std::cout << "op_node_" << id_map[node->identifier_idx];
	}
	else if (node->node_type == node_ifthenelse) {
		std::cout << "ifthenelse";
	}
}

void dfs(struct ast_node *node)
{
	for (int i = 0; i < 3; i++)
	{
		if (node->children[i] != NULL && (node->node_type == node_app || node->node_type == node_lamda || node->node_type == node_op
		|| node->node_type == node_ifthenelse))
		{
			std::cout << node << '_';
			print_node(node);
			std::cout << " -> ";
			std::cout << node->children[i] << '_';
			print_node(node->children[i]);
			std::cout << std::endl;
			dfs(node->children[i]);
		}
	}
}

int main()
{
	int current_token;
	while ((current_token = get_token()) != tok_eof)
	{
		token_stream.push_back(current_token);
		current_token_idx++;
	}
	int i = 0;

	struct ast_node *ast;
	int si = parsing_idx;
	ast = parse_term();
	// Minimal parsing required for single evaluation is complete as of 19:42 10th Jan
	// TODO: implement semantics from the rewrite rules given
	if (ast != NULL)
	{
		dfs(ast);
	}
	else
	{
		std::cout << "Error in parsing\n";
	}
	return 0;
}
