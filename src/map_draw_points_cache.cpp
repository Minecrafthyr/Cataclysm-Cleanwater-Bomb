#include "map.h"

// Out-of-line operator[] definition for draw_points_cache_t::level_rows
draw_points_cache_t::level_rows::row_vec &draw_points_cache_t::level_rows::operator[]( const int row )
{
    if( !initialized ) {
        row_base = row;
        initialized = true;
    }
    if( row < row_base ) {
        // Prepend empty rows. tile_render_info has a const member
        // (deleted copy/move assignment), so vector::insert  which
        // shifts via assignment  won't compile. Rebuild front-to-back
        // using move-construction only.
        std::vector<row_vec> grown;
        grown.reserve( rows.size() + static_cast<size_t>( row_base - row ) );
        grown.resize( static_cast<size_t>( row_base - row ) );
        for( row_vec &r : rows ) {
            grown.push_back( std::move( r ) );
        }
        rows.swap( grown );
        row_base = row;
    }
    const size_t idx = static_cast<size_t>( row - row_base );
    if( idx >= rows.size() ) {
        rows.resize( idx + 1 );
    }
    return rows[idx];
}
