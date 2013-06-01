/**
 * \class	IdTable
 * 
 * \ingroup	containers
 * \brief	
 * \author	Mattias Gustavsson
 * 
 */

#ifndef __IdTable_H__
#define __IdTable_H__

// Includes
#include "PodCheck.h"
#include "StandardLibrary.h"
#include "Debug.h"

// Forward declares
typedef unsigned int Id;

// IdTable
template< typename T, POD_ENUM POD = NOT_POD, unsigned char COUNTER_BIT_COUNT = 12, unsigned char INDEX_BIT_COUNT = 20 >
class IdTable
    {
    public:
        //*** Constructor ***
        
        IdTable( unsigned int initialCapacity = 64 ):
            count_( 0 ),
            capacity_( initialCapacity ),
            usedIndicesCount_( 0 ),
            freeListHead_( 0 ),
            freeListTail_( 0 ),
            objects_( 0 ),
            indices_( 0 ),
            objectSlotToIndex_( 0 ),
            freeList_( 0 )           
            {
            COMPILE_CHECK_IS_POD( T, POD );
            }
            
            
        //*** Destructor ***
        
        ~IdTable()
            {
            if( freeList_ )
                {
                Free( freeList_ );
                }

            if( objectSlotToIndex_ )
                {
                Free( objectSlotToIndex_ );
                }
            
            if( indices_ )
                {
                Free( indices_ );
                }

            if( objects_ )
                {
                if( POD == NOT_POD )
                    {
                    T* op = objects_;
                    for( unsigned int i = 0; i < count_; i++ )
                        {
                        op->~T();
                        op++;
                        }
                    } 
                Free( objects_ );
                }
            }


        //*** Add ***

        Id Add( const T& object = T() )
            {
            if( !objects_ )            
                {
                objects_ = (T*) Malloc( sizeof( T ) * capacity_ );
                }
                
            if( !indices_ )            
                {
                indices_ = (Index*) Malloc( sizeof( Index ) * capacity_ );
                }
                
            if( !objectSlotToIndex_ )            
                {
                objectSlotToIndex_ = (unsigned int*) Malloc( sizeof( unsigned int* ) * capacity_ );
                }
                
            if( !freeList_ )            
                {
                freeList_ = (unsigned int*) Malloc( sizeof( unsigned int* ) * capacity_ );
                }
                
            if( count_ >= ( capacity_ - ( capacity_  / 4 ) ) )
                {
                capacity_ *= 2;
                if( POD == NOT_POD )
                    {
                    T* newObjects = (T*) Malloc( sizeof( T ) * capacity_ );
                    T* np = newObjects;
                    T* op = objects_;
                    for( unsigned int i = 0; i < count_; i++ )
                        {
                        new (np) T( *op );
                        np++;
                        op++;
                        }
                    Free( objects_ );
                    objects_ = newObjects;                    
                    }
                else
                    {
                    objects_ = (T*) Realloc( objects_, sizeof( T ) * capacity_ );                        
                    }
                indices_ = (Index*) Realloc( indices_, sizeof( Index ) * capacity_ );
                objectSlotToIndex_ = (unsigned int*) Realloc( objectSlotToIndex_, sizeof( Index ) * capacity_ );
                freeList_ = (unsigned int*) Realloc( freeList_, sizeof( Index ) * capacity_ );
                }

            unsigned int newIndex = (unsigned int) -1;
            if( usedIndicesCount_ >= capacity_ )
                {
                Assert( freeListHead_ != freeListTail_, "Freelist corrupted" );
                newIndex = freeList_[ freeListHead_ ];
                freeListHead_++;
                if( freeListHead_ >= capacity_ )
                    {
                    freeListHead_ = 0;
                    }
                }
            else
                {
                newIndex = usedIndicesCount_;
                usedIndicesCount_++;
                indices_[ newIndex ].counter = 0;
                }

            indices_[ newIndex ].objectIndex = count_;
            
            if( POD == NOT_POD )
                {
                new (&objects_[ count_ ]) T( object );
                }
            else
                {
                objects_[ count_ ] = object;
                }
            objectSlotToIndex_[ count_ ] = newIndex;
            
            Id id = ToId( indices_[ newIndex ].counter, newIndex );
            
            count_++;
            return id;
            }

            
        //*** Remove ***

        void Remove( Id id )
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex >= capacity_ )
                {
                return;
                }
                
            Index index = indices_[ idIndex ];
            
            if( index.counter != idCounter || index.objectIndex >= count_ )
                {
                return;
                }
            
            objects_[ index.objectIndex ].~T();
            if( count_ > 0 && index.objectIndex < count_ - 1 )
                {
                if( POD == NOT_POD )                
                    {
                    new ( &objects_[ index.objectIndex ]) T( objects_[ count_ -1 ] );
                    objects_[ count_ - 1 ].~T();
                    }
                else
                    {
                    objects_[ index.objectIndex ] = objects_[ count_ - 1 ];
                    }
                
                indices_[ objectSlotToIndex_[ count_ - 1 ] ].objectIndex = indices_[ objectSlotToIndex_[ index.objectIndex ] ].objectIndex;  
                objectSlotToIndex_[ index.objectIndex ] =  objectSlotToIndex_[ count_ -1 ];
                }
                
            count_--;                
            
            indices_[ idIndex ].counter++;
            freeList_[ freeListTail_ ] = idIndex;
            freeListTail_ ++;
            if( freeListTail_ >= capacity_ )
                {
                freeListTail_ = 0;
                }
            }

        
        //*** Get ***

        const T& Get( Id id ) const
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    return objects_[ index.objectIndex ];
                    }
                }
                    
            Assert( false, "Invalid id");                    
            static T defaultValue;
            return defaultValue; 
            }


        //*** Get ***

        T& Get( Id id )
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    return objects_[ index.objectIndex ];
                    }
                }
                    
            Assert( false, "Invalid id");                    
            static T defaultValue;
            return defaultValue; 
            }


        //*** Set ***

        void Set( Id id, const T& object )
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    objects_[ index.objectIndex ] = object;
                    }
                }
                
            }


        //*** IsValid ***

        bool IsValid( Id id ) const
            {
            unsigned int idCounter;
            unsigned int idIndex;
            FromId( id, idCounter, idIndex );

            if( idIndex < capacity_ )
                {
                Index index = indices_[ idIndex ];
                
                if( index.counter == idCounter && index.objectIndex < count_ )
                    {
                    return true;
                    }
                }
            
            return false;
            }
            

        //*** GetItemCount ***

        int GetItemCount() const
            {
            return count_;
            }           


        //*** Get ***

        const T& Get( int index ) const
            {
            if ( index < 0 || (unsigned int) index >= count_ )
                {
                Assert( false, "Index out of range");                    
                static T defaultValue;
                return defaultValue; 
                }
            return objects_[ index ];            
            }           


        //*** Get ***

        T& Get( int index )
            {
            if ( index < 0 || (unsigned int) index >= count_ )
                {
                Assert( false, "Index out of range");                    
                static T defaultValue;
                return defaultValue; 
                }
            return objects_[ index ];            
            }           


        //*** GetId ***

        Id GetId( int index ) const
            {
            if ( index < 0 || (unsigned int) index >= count_ )
                {
                Assert( false, "Index out of range");                    
                Id id = ToId( (unsigned int) -1, (unsigned int)-1 );
                return id;
                }
                
            unsigned int actualIndex = objectSlotToIndex_[ index ];                
            Id id = ToId( indices_[ actualIndex ].counter, actualIndex );
            return id;            
            }           

    private:

        //*** ToId ***

        Id ToId( unsigned int counter, unsigned int index ) const
            {
            const unsigned int COUNTER_BIT_MASK = ( 1 << COUNTER_BIT_COUNT ) - 1;
            const unsigned int INDEX_BIT_MASK = ( 1 << INDEX_BIT_COUNT ) - 1;

            Id id = index & INDEX_BIT_MASK;
            id |= ( counter & COUNTER_BIT_MASK ) << INDEX_BIT_COUNT;
            return id;
            }

        
        //*** FromId ***

        void FromId( Id id, unsigned int& counter, unsigned int& index ) const
            {
            const unsigned int COUNTER_BIT_MASK = ( 1 << COUNTER_BIT_COUNT ) - 1;
            const unsigned int INDEX_BIT_MASK = ( 1 << INDEX_BIT_COUNT ) - 1;

            counter = ( id  >> INDEX_BIT_COUNT ) & COUNTER_BIT_MASK;
            index = id & INDEX_BIT_MASK;
            }
            
    private:      
        struct Index
            {
            unsigned int counter : COUNTER_BIT_COUNT;
            unsigned int objectIndex : INDEX_BIT_COUNT;
            };
            
        unsigned int capacity_;
        unsigned int count_;
	    T* objects_;
	    Index* indices_;
	    unsigned int* objectSlotToIndex_;
	    unsigned int* freeList_;
	    unsigned int freeListHead_;
	    unsigned int freeListTail_;
        unsigned int usedIndicesCount_;
    };

// Implementation
#include "IdTable.inl"

#endif /* __IdTable_h__ */
