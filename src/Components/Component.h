#pragma once

struct Component {
	virtual ~Component() { };
};

#define MAKE_COMP(name) \
	struct name : public Component { \
		int x, y, z, w, r, t; \
	};

MAKE_COMP(A)
MAKE_COMP(B)
MAKE_COMP(C)
MAKE_COMP(D)
MAKE_COMP(E)
MAKE_COMP(F)
MAKE_COMP(G)
MAKE_COMP(H)
MAKE_COMP(I)
MAKE_COMP(J)
MAKE_COMP(K)
MAKE_COMP(L)
MAKE_COMP(M)
MAKE_COMP(N)
MAKE_COMP(O)
MAKE_COMP(P)
MAKE_COMP(Q)
MAKE_COMP(R)
MAKE_COMP(S)
