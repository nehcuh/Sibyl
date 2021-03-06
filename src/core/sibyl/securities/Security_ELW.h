/*
   Copyright 2017 Hosang Yoon

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef SIBYL_SECURITY_ELW_
#define SIBYL_SECURITY_ELW_

#include <cmath>

#include "../Security.h"

namespace sibyl
{

enum class OptType { null =  0,
                     call = +1,   // value specified in comm_format.txt
                     put  = -1 }; // value specified in comm_format.txt

template <class TItem>
class ELW : public TItem // derive from a specialized Security<TOrder> (i.e., Item)
{
public:
    // virtuals from Security
    SecType Type  ()        const { return SecType::ELW;                }
    INT     TckHi (INT p)   const { return p + 5;                       }
    INT     TckLo (INT p)   const { return (p - 5) * (p > 0);           }
    bool    ValidP(INT p)   const { return (p > 0) && ((p % 5) == 0);   }
    INT64   BFee  (INT64 r) const { return (INT64)std::round(r * dBF0); }
    INT64   SFee  (INT64 r) const { return (INT64)std::round(r * dSF0); }
    double  dBF   ()        const { return dBF0;                        }
    double  dSF   ()        const { return dSF0;                        }
    
    // additionals
    static FLOAT kospi200;

    constexpr static std::ptrdiff_t szTh = 8;  // number of fields in theoretical Greeks
    std::array<FLOAT, szTh>         thr;

    void    SetInfo(OptType optType_, INT expiry_) {
        verify((optType_ == OptType::call || optType_ == OptType::put) && expiry_ >= 0);
        optType = optType_; expiry = expiry_;
    }
    OptType CallPut() const { verify(optType != OptType::null); return optType; }
    INT     Expiry () const { verify(optType != OptType::null); return expiry;  }
    
    ELW() : thr{}, optType(OptType::null), expiry(0) {}
    ELW(OptType t, INT e) : thr{} { SetInfo(t, e); }
private:
    constexpr static const double dBF0 = 0.00015;
    constexpr static const double dSF0 = 0.00015;
    OptType optType;
    INT     expiry;
};
    
template <class TItem>
FLOAT ELW<TItem>::kospi200 = (FLOAT) std::nan("");

}

#endif /* SIBYL_SECURITY_ELW_ */