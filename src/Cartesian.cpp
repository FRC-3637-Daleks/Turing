#include "Cartesian.h"

Cartesian::Cartesian(const Cartesian &other): m_x(other.m_x), m_y(other.m_y), m_z(other.m_z) {};

void Cartesian::operator() (const double ix, const double iy, const double iz)
{
    m_x = ix;
    m_y = iy;
    m_z = iz;
}

Cartesian& Cartesian::operator= (const Cartesian& other)
{
    m_x = other.m_x;
    m_y = other.m_y;
    m_z = other.m_z;
}

Cartesian& Cartesian::add(const Cartesian &other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
}

const Cartesian Cartesian::Add(const Cartesian& lArg, const Cartesian &rArg)
{
	return Cartesian(lArg.m_x + rArg.m_x, lArg.m_y + rArg.m_y, lArg.m_z + rArg.m_z);
}

const Cartesian Cartesian::Diff(const Cartesian& lArg, const Cartesian &rArg)
{
	return Cartesian(lArg.m_x - rArg.m_x, lArg.m_y - rArg.m_y, lArg.m_z - rArg.m_z);
}
