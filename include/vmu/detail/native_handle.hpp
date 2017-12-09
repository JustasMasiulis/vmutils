/*
* Copyright 2017 Justas Masiulis
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef VMU_NATIVE_HANDLES
#define VMU_NATIVE_HANDLES

/// \brief The native, operating system dependant handle
#if defined(_WIN32)
	using native_handle_t = void*;
#elif defined(__APPLE__)
	#include <mach/port.h>
	using native_handle_t = ::mach_port_t;
#else
    #include <sys/types.h>
	using native_handle_t = ::pid_t;
#endif

#endif // include guard