#include "../../naraapi.h"


void Nara::LL::Package::GetBytes(uint8_t *bytes) { /* Nothing should happen here */ }
void Nara::LL::Package::LoadBytes(uint8_t *bytes) { /* Nothing should happen here */ }

uint16_t Nara::LL::Package::GetLength()
{
	return length;
}