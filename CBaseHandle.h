#pragma once

// cbf creating two different header - oops

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/game/shared/ehandle.h
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/basehandle.h

#define NUM_ENT_ENTRY_BITS         ( 11 + 2 )
#define NUM_ENT_ENTRIES            ( 1 << NUM_ENT_ENTRY_BITS )
#define INVALID_EHANDLE_INDEX      0xFFFFFFFF
#define NUM_SERIAL_NUM_BITS        16
#define NUM_SERIAL_NUM_SHIFT_BITS  ( 32 - NUM_SERIAL_NUM_BITS )
#define ENT_ENTRY_MASK             ( ( 1 << NUM_SERIAL_NUM_BITS ) - 1)

class CBaseHandle;

class IHandleEntity {
public:
	virtual ~IHandleEntity( ) { }
	virtual void set_handle( const CBaseHandle& handle ) = 0;
	virtual const CBaseHandle& get_handle( ) const = 0;
};

class CBaseHandle {
public:
	CBaseHandle( );
	CBaseHandle( const CBaseHandle& other );
	CBaseHandle( unsigned long value );
	CBaseHandle( int iEntry, int iSerialNumber );

	void init( int iEntry, int iSerialNumber );
	void term( );

	// even if this returns true, Get() still can return return a non-null value.
	// this just tells if the handle has been initted with any values.
	bool is_valid( ) const;

	int get_entry_index( ) const;
	int get_serial_number( ) const;

	int to_int( ) const;
	bool operator !=( const CBaseHandle& other ) const;
	bool operator ==( const CBaseHandle& other ) const;
	bool operator ==( const IHandleEntity* pEnt ) const;
	bool operator !=( const IHandleEntity* pEnt ) const;
	bool operator <( const CBaseHandle& other ) const;
	bool operator <( const IHandleEntity* pEnt ) const;

	// assign a value to the handle.
	const CBaseHandle& operator=( const IHandleEntity* pEntity );
	const CBaseHandle& set( const IHandleEntity* pEntity );

	IHandleEntity* get( ) const;

protected:
	unsigned long index;
};

template< class T >
class c_handle : public CBaseHandle {
public:
	c_handle( );
	c_handle( int iEntry, int iSerialNumber );
	c_handle( const CBaseHandle& handle );
	c_handle( T* pVal );

	static c_handle< T > from_index( int index );

	T* get( ) const;
	void set( const T* pVal );

	operator T* ( );
	operator T* ( ) const;

	bool operator!( ) const;
	bool operator==( T* val ) const;
	bool operator!=( T* val ) const;
	const CBaseHandle& operator=( const T* val );

	T* operator->( ) const;
};

template< class T >
c_handle< T >::c_handle( ) {}

template< class T >
c_handle< T >::c_handle( int iEntry, int iSerialNumber ) {
	init( iEntry, iSerialNumber );
}

template< class T >
c_handle< T >::c_handle( const CBaseHandle& handle )
	: CBaseHandle( handle ) {}

template< class T >
c_handle< T >::c_handle( T* pObj ) {
	term( );
	set( pObj );
}

template< class T >
c_handle< T > c_handle< T >::from_index( int index ) {
	c_handle< T > ret;
	ret.index = index;
	return ret;
}

template< class T >
T* c_handle< T >::get( ) const {
	return reinterpret_cast< T* >( CBaseHandle::get( ) );
}

template< class T >
c_handle< T >::operator T* ( ) {
	return get( );
}

template< class T >
c_handle< T >::operator T* ( ) const {
	return get( );
}

template< class T >
bool c_handle< T >::operator !( ) const {
	return !get( );
}

template< class T >
bool c_handle< T >::operator==( T* val ) const {
	return get( ) == val;
}

template< class T >
bool c_handle< T >::operator!=( T* val ) const {
	return get( ) != val;
}

template< class T >
void c_handle< T >::set( const T* pVal ) {
	CBaseHandle::set( reinterpret_cast< const IHandleEntity* >( pVal ) );
}

template< class T >
const CBaseHandle& c_handle< T >::operator=( const T* val ) {
	set( val );
	return *this;
}

template< class T >
T* c_handle< T >::operator ->( ) const {
	return get( );
}

inline CBaseHandle::CBaseHandle( ) {
	index = INVALID_EHANDLE_INDEX;
}

inline CBaseHandle::CBaseHandle( const CBaseHandle& other ) {
	index = other.index;
}

inline CBaseHandle::CBaseHandle( unsigned long value ) {
	index = value;
}

inline CBaseHandle::CBaseHandle( int iEntry, int iSerialNumber ) {
	init( iEntry, iSerialNumber );
}

inline void CBaseHandle::init( int iEntry, int iSerialNumber ) {
	index = static_cast< unsigned long >( iEntry | ( iSerialNumber << NUM_SERIAL_NUM_SHIFT_BITS ) );
}

inline void CBaseHandle::term( ) {
	index = INVALID_EHANDLE_INDEX;
}

inline bool CBaseHandle::is_valid( ) const {
	return index != INVALID_EHANDLE_INDEX;
}

inline int CBaseHandle::get_entry_index( ) const {
	if ( !is_valid( ) )
		return NUM_ENT_ENTRIES - 1;

	return index & ENT_ENTRY_MASK;
}

inline int CBaseHandle::get_serial_number( ) const {
	return index >> NUM_SERIAL_NUM_SHIFT_BITS;
}

inline int CBaseHandle::to_int( ) const {
	return static_cast< int >( index );
}

inline bool CBaseHandle::operator !=( const CBaseHandle& other ) const {
	return index != other.index;
}

inline bool CBaseHandle::operator ==( const CBaseHandle& other ) const {
	return index == other.index;
}

inline bool CBaseHandle::operator ==( const IHandleEntity* pEnt ) const {
	return get( ) == pEnt;
}

inline bool CBaseHandle::operator !=( const IHandleEntity* pEnt ) const {
	return get( ) != pEnt;
}

inline bool CBaseHandle::operator <( const CBaseHandle& other ) const {
	return index < other.index;
}

inline bool CBaseHandle::operator <( const IHandleEntity* pEntity ) const {
	unsigned long otherIndex = ( pEntity ) ? pEntity->get_handle( ).index : INVALID_EHANDLE_INDEX;
	return index < otherIndex;
}

inline const CBaseHandle& CBaseHandle::operator=( const IHandleEntity* pEntity ) {
	return set( pEntity );
}

inline const CBaseHandle& CBaseHandle::set( const IHandleEntity* pEntity ) {
	if ( pEntity )
		* this = pEntity->get_handle( );
	else
		index = INVALID_EHANDLE_INDEX;

	return *this;
}