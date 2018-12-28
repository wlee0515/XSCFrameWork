#ifndef XSC_SPACE_MODEL
#define XSC_SPACE_MODEL

#include <map>

namespace XSC
{

  namespace tools
  {
    template<typename T_Key>
    class SpaceModel
    {
    public:
      typedef size_t Index;
      typedef std::map<Index, T_Key> Space_1D;
      typedef std::map<Index, Space_1D> Space_2D;
      typedef std::map<Index, Space_2D> Space_3D;
      typedef std::map<Index, Space_3D> Space_4D;

      static const T_Key* searchSpace(const Index& iIndex1, const Space_1D& iSpace)
      {
        Space_1D::const_iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static T_Key* searchSpace(const Index& iIndex1, Space_1D& iSpace)
      {
        Space_1D::iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static const Space_1D* searchSpace(const Index& iIndex1, const Space_2D& iSpace)
      {
        Space_2D::const_iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static Space_1D* searchSpace(const Index& iIndex1, Space_2D& iSpace)
      {
        Space_2D::iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static const T_Key* searchSpace(const Index& iIndex1, const Index& iIndex2, const Space_2D& iSpace)
      {
        const Space_1D* wSpace_1D = searchSpace(iIndex1, iSpace);

        if (nullptr !+ wSpace_1D)
        {
          return searchSpace(iIndex2, *wSpace_1D);
        }

        return nullptr;
      }

      static T_Key* searchSpace(const Index& iIndex1, const Index& iIndex2, Space_2D& iSpace)
      {
        Space_1D* wSpace_1D = searchSpace(iIndex1, iSpace);

        if (nullptr !+ wSpace_1D)
        {
          return searchSpace(iIndex2, *wSpace_1D);
        }

        return nullptr;
      }

      static const Space_2D* searchSpace(const Index& iIndex1, const Space_3D& iSpace)
      {
        Space_3D::const_iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static Space_2D* searchSpace(const Index& iIndex1, Space_3D& iSpace)
      {
        Space_3D::iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static const Space_1D* searchSpace(const Index& iIndex1, const Index& iIndex2, const Space_3D& iSpace)
      {
        const Space_2D* wSpace_2D = searchSpace(iIndex1, iSpace);

        if (nullptr != wSpace_2D)
        {
          return searchSpace(iIndex2, *wSpace_2D);
        }

        return nullptr;
      }

      static Space_1D* searchSpace(const Index& iIndex1, const Index& iIndex2, Space_3D& iSpace)
      {
        Space_2D* wSpace_2D = searchSpace(iIndex1, iSpace);

        if (nullptr != wSpace_2D)
        {
          return searchSpace(iIndex2, *wSpace_2D);
        }

        return nullptr;
      }

      static const T_Key* searchSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, const Space_3D& iSpace)
      {
        const Space_1D* wSpace_1D = searchSpace(iIndex1, iIndex2, iSpace);

        if (nullptr != wSpace_1D)
        {
          return searchSpace(iIndex3, *wSpace_1D);
        }

        return nullptr;
      }

      static T_Key* searchSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, Space_3D& iSpace)
      {
        Space_1D* wSpace_1D = searchSpace(iIndex1, iIndex2, iSpace);

        if (nullptr != wSpace_1D)
        {
          return searchSpace(iIndex3, *wSpace_1D);
        }

        return nullptr;
      }

      static const Space_3D* searchSpace(const Index& iIndex1, const Space_4D& iSpace)
      {
        Space_4D::const_iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static Space_3D* searchSpace(const Index& iIndex1, Space_4D& iSpace)
      {
        Space_4D::iterator wIt = iSpace.find(iIndex1);

        if (iSpace.end() != wIt)
        {
          return &wIt->second;
        }

        return nullptr;
      }

      static const Space_2D* searchSpace(const Index& iIndex1, const Index& iIndex2, const Space_4D& iSpace)
      {
        const Space_3D* wSpace_3D = searchSpace(iIndex1, iSpace);

        if (nullptr != wSpace_3D)
        {
          return searchSpace(iIndex2, *wSpace_3D);
        }

        return nullptr;
      }

      static Space_2D* searchSpace(const Index& iIndex1, const Index& iIndex2, Space_4D& iSpace)
      {
        Space_3D* wSpace_3D = searchSpace(iIndex1, iSpace);

        if (nullptr != wSpace_3D)
        {
          return searchSpace(iIndex2, *wSpace_3D);
        }

        return nullptr;
      }

      static const Space_1D* searchSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, const Space_4D& iSpace)
      {
        const Space_2D* wSpace_2D = searchSpace(iIndex1, iIndex2, iSpace);

        if (nullptr != wSpace_2D)
        {
          return searchSpace(iIndex3, *wSpace_2D);
        }

        return nullptr;
      }

      static Space_1D* searchSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, Space_4D& iSpace)
      {
        Space_2D* wSpace_2D = searchSpace(iIndex1, iIndex2, iSpace);

        if (nullptr != wSpace_2D)
        {
          return searchSpace(iIndex3, *wSpace_2D);
        }

        return nullptr;
      }

      static const T_Key* searchSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, const Index& iIndex4, const Space_4D& iSpace)
      {
        const Space_1D* wSpace_1D = searchSpace(iIndex1, iIndex2, iIndex3, iSpace);

        if (nullptr != wSpace_1D)
        {
          return searchSpace(iIndex4, *wSpace_1D);
        }

        return nullptr;
      }

      static T_Key* searchSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, const Index& iIndex4, Space_4D& iSpace)
      {
        Space_1D* wSpace_1D = searchSpace(iIndex1, iIndex2, iIndex3, iSpace);

        if (nullptr != wSpace_1D)
        {
          return searchSpace(iIndex4, *wSpace_1D);
        }

        return nullptr;
      }

      static void clearSpace(const Index& iIndex1, Space_1D& iSpace)
      {
        iSpace.erase(iIndex1);
      }

      static void clearSpace(const Index& iIndex1, Space_2D& iSpace)
      {
        iSpace.erase(iIndex1);
      }

      static void clearSpace(const Index& iIndex1, const Index& iIndex2, Space_2D& iSpace)
      {
        const Space_1D* wSpace_1D = searchSpace(iIndex1, iSpace);

        if (nullptr != wSpace_1D)
        {
          clearSpace(iIndex2, *wSpace_1D);

          if (0 == wSpace_1D->size())
          {
            clearSpace(iIndex1, iSpace);
          }
        }
      }

      static void clearSpace(const Index& iIndex1, Space_3D& iSpace)
      {
        iSpace.erase(iIndex1);
      }

      static void clearSpace(const Index& iIndex1, const Index& iIndex2, Space_3D& iSpace)
      {
        const Space_2D* wSpace_2D = searchSpace(iIndex1, iSpace);

        if (nullptr != wSpace_2D)
        {
          clearSpace(iIndex2, *wSpace_2D);

          if (0 == wSpace_2D->size())
          {
            clearSpace(iIndex1, iSpace);
          }
        }
      }


      static void clearSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, Space_3D& iSpace)
      {
        const Space_1D* wSpace_1D = searchSpace(iIndex1, iIndex2, iSpace);

        if (nullptr != wSpace_1D)
        {
          clearSpace(iIndex3, *wSpace_1D);

          if (0 == wSpace_1D->size())
          {
            clearSpace(iIndex1, iIndex2, iSpace);
          }
        }
      }

      static void clearSpace(const Index& iIndex1, Space_4D& iSpace)
      {
        iSpace.erase(iIndex1);
      }

      static void clearSpace(const Index& iIndex1, const Index& iIndex2, Space_4D& iSpace)
      {
        const Space_3D* wSpace_3D = searchSpace(iIndex1, iSpace);

        if (nullptr != wSpace_3D)
        {
          clearSpace(iIndex2, *wSpace_3D);

          if (0 == wSpace_3D->size())
          {
            clearSpace(iIndex1, iSpace);
          }
        }
      }

      static void clearSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, Space_4D& iSpace)
      {
        const Space_2D* wSpace_2D = searchSpace(iIndex1, iIndex2, iSpace);

        if (nullptr != wSpace_2D)
        {
          clearSpace(iIndex3, *wSpace_2D);

          if (0 == wSpace_2D->size())
          {
            clearSpace(iIndex1, iIndex2, iSpace);
          }
        }
      }

      static void clearSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, const Index& iIndex4, Space_4D& iSpace)
      {
        const Space_1D* wSpace_1D = searchSpace(iIndex1, iIndex2, iIndex3, iSpace);

        if (nullptr != wSpace_1D)
        {
          clearSpace(iIndex4, *wSpace_1D);

          if (0 == wSpace_1D->size())
          {
            clearSpace(iIndex1, iIndex2, iIndex3, iSpace);
          }
        }
      }

      static T_Key* setSpace(const Index& iIndex1, Space_1D& iSpace, const T_Key iKey)
      {
        return &(iSpace[iIndex1] = iKey);
      }

      static T_Key* setSpace(const Index& iIndex1, const Index& iIndex2, Space_2D& iSpace, const T_Key iKey)
      {
        return &(iSpace[iIndex1][iIndex2] = iKey);
      }

      static T_Key* setSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, Space_3D& iSpace, const T_Key iKey)
      {
        return &(iSpace[iIndex1][iIndex2][iIndex3] = iKey);
      }

      static T_Key* setSpace(const Index& iIndex1, const Index& iIndex2, const Index& iIndex3, const Index& iIndex4, Space_4D& iSpace, const T_Key iKey)
      {
        return &(iSpace[iIndex1][iIndex2][iIndex3][iIndex4] = iKey);
      }
    };
  }
}

#endif