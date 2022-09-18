/*
  ==============================================================================

    EdPF_GrainPool.h
    Created: 25 Aug 2022 8:40:40pm
    Author:  Edward Gallyot

  ==============================================================================
*/

#ifndef EDPF_GRAIN_POOL_H_INCLUDED
#define EDPF_GRAIN_POOL_H_INCLUDED

#include "EdPF_Grain.h"


namespace EdPF
{
    namespace Grains
    {
        // Provides an object pool for a series of grains, unfortunately it doesn't
        // work very efficiently at the moment.
        // TODO: Make this pool more efficient.
        template <typename GrainT>
        class GrainPool
        {
        public:
            GrainPool(size_t size) : 
                m_pool(size),
                m_size(size)
            {
                for (auto& grain : m_pool)
                {
                    grain.SetIsActive(false);
                }
            }
            
            std::vector<GrainT>& GetPool()
            {
                return m_pool;
            }

            // Always check for nullptr when using this!!
            // It'll look for a pointer to a free grain but if we can'y 
            GrainT* FindNextFreeGrain()
            {
                for (auto& grain : m_pool)
                {
                    if (!grain.GetIsActive())
                    {
                        return &grain;
                    }
                }
                return nullptr;
            }

        private:
            std::vector<GrainT> m_pool;
            size_t m_size;
        };
    }
}

#endif //!EDPF_GRAIN_POOL_H_INCLUDED
