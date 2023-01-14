#include "Scanner.h"

bool operator==(const PPWord& first, const PPWord second)
{
	return (first.lexeme == second.lexeme && first.token == second.token);
}
