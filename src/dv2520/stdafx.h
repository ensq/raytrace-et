#ifndef DV2520_STDAFX_H
#define DV2520_STDAFX_H

#define NOMINMAX

// Std:
#include <map>
#include <queue>
#include <math.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <cassert>
#include <iostream>
#include <typeinfo>
#include <windows.h>
#include <exception>

// Dx:
#include <d3d11.h>
#include <D3D11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

// Windows:
#include <comdef.h>
#include <strsafe.h>
#include <windows.h>
#include <windowsx.h>

// Application:
#include <Util.h> // Utility methods included for simplicity.
#include <MathSimple.h> // Math methods included for simplicity.
#include <ExceptionDv2520.h>

// Conditionals:
#ifdef DV2520_DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // DV2520_DEBUG

namespace DxClearColor {    
    const FLOAT Red  [4] = {1.0f, 0.0f, 0.0f, 1.0f};
    const FLOAT Green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    const FLOAT Blue [4] = {0.0f, 0.0f, 1.0f, 1.0f};
    const FLOAT Black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    const FLOAT White[4] = {1.0f, 1.0f, 1.0f, 1.0f};
}

#endif // DV2520_STDAFX_H
