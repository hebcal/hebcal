#     $Header$
#     Hebcal - A Jewish Calendar Generator
#     Copyright (C) 1994-1998  Danny Sadinoff

#     This program is free software; you can redistribute it and/or
#     modify it under the terms of the GNU General Public License
#     as published by the Free Software Foundation; either version 2
#     of the License, or (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#     Danny Sadinoff can be reached at 
#     1 Cove La.
#     Great Neck, NY
#     11024

#     sadinoff@pobox.com
#     http://pobox.com/~sadinoff

package Date;
use Carp;

my @FIELDNAMES = qw( yy mm dd );

# 5.005 feature
# use fields qw( yy mm dd  ); #neat-o compile-time field-name checking

#months are 1-based    
sub new (@)
{ 
   my $class = shift; 
   my $self;
   if( ref $_[0] eq 'Date' )
   {
      $self = bless { map {$_,($_[0]->{$_})} @FIELDNAMES };
   }
   else
   {
      scalar(@_) %2 == 0 or croak "bad arglist";
      my %args = @_;
      $self = bless { map {$_,( $args{$_} ) } @FIELDNAMES };
   }
   return $self;
}

1;
