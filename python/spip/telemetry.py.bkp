#!/usr/bin/env python3
"""
Auto-generating Telemetry Decoder for Instrument Status

CONFIGURATION: Paste your C++ header definitions below
"""

# ============================================================================
# HEADER DEFINITIONS - Default configuration (can be overridden)
# ============================================================================

# Default header files (optional - can be overridden in create_decoder)
HEADER_FILES = [
    # "telemetry_t.h",
]

# Default main struct (optional - can be overridden in create_decoder)
MAIN_STRUCT = None

# ============================================================================
# AUTO-GENERATION CODE - Do not modify below this line
# ============================================================================

import base64
import struct
import re
from dataclasses import dataclass, field
from typing import Dict, List, Tuple, Optional, Any


@dataclass
class StructField:
    """Represents a field in a C++ struct"""
    name: str
    type: str
    is_array: bool = False
    array_size: int = 0
    is_pointer: bool = False


@dataclass
class StructDef:
    """Represents a C++ struct definition"""
    name: str
    fields: List[StructField] = field(default_factory=list)


class StructParser:
    """Parses C++ header content to extract struct definitions"""
    
    # Map C++ types to Python struct format and size
    TYPE_MAP = {
        'int': ('i', 4),
        'unsigned int': ('I', 4),
        'uint': ('I', 4),
        'short': ('h', 2),
        'unsigned short': ('H', 2),
        'ushort': ('H', 2),
        'long': ('q', 8),
        'unsigned long': ('Q', 8),
        'long long': ('q', 8),
        'float': ('f', 4),
        'double': ('d', 8),
        'char': ('c', 1),
        'signed char': ('b', 1),
        'unsigned char': ('B', 1),
        'uchar': ('B', 1),
        'bool': ('?', 1),
        '_Bool': ('?', 1),
        # Fixed-width integer types (signed and unsigned)
        'int8_t': ('b', 1),
        'uint8_t': ('B', 1),
        'int16_t': ('h', 2),
        'uint16_t': ('H', 2),
        'int32_t': ('i', 4),
        'uint32_t': ('I', 4),
        'int64_t': ('q', 8),
        'uint64_t': ('Q', 8),
    }
    
    def __init__(self):
        self.structs: Dict[str, StructDef] = {}
        self.typedefs: Dict[str, str] = {}
    
    def parse(self, content: str):
        """Parse C++ header content and extract struct definitions"""
        # Remove comments
        content = re.sub(r'//.*?\n', '\n', content)
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)
        
        # Find typedef struct patterns
        typedef_pattern = r'typedef\s+struct\s*\{([^}]+)\}\s*(\w+)\s*;'
        for match in re.finditer(typedef_pattern, content, re.DOTALL):
            struct_body = match.group(1)
            struct_name = match.group(2)
            fields = self._parse_struct_body(struct_body)
            self.structs[struct_name] = StructDef(struct_name, fields)
        
        # Find regular struct patterns
        struct_pattern = r'struct\s+(\w+)\s*\{([^}]+)\}\s*;'
        for match in re.finditer(struct_pattern, content, re.DOTALL):
            struct_name = match.group(1)
            struct_body = match.group(2)
            fields = self._parse_struct_body(struct_body)
            self.structs[struct_name] = StructDef(struct_name, fields)
        
        # Find simple typedefs
        typedef_simple = r'typedef\s+(\w+)\s+(\w+)\s*;'
        for match in re.finditer(typedef_simple, content):
            self.typedefs[match.group(2)] = match.group(1)
    
    def _parse_struct_body(self, body: str) -> List[StructField]:
        """Parse the body of a struct to extract fields"""
        fields = []
        
        # Split by semicolon to get individual field declarations
        declarations = body.split(';')
        
        for decl in declarations:
            decl = decl.strip()
            if not decl or decl.startswith('#'):
                continue
            
            # Parse field: type name[array_size] or type name
            # Handle arrays: char user[128]
            array_match = re.search(r'(\w+(?:\s+\w+)*)\s+(\w+)\s*\[(\d+)\]', decl)
            if array_match:
                type_name = array_match.group(1).strip()
                field_name = array_match.group(2)
                array_size = int(array_match.group(3))
                fields.append(StructField(
                    name=field_name,
                    type=type_name,
                    is_array=True,
                    array_size=array_size
                ))
                continue
            
            # Handle pointers
            pointer_match = re.search(r'(\w+(?:\s+\w+)*)\s*\*+\s*(\w+)', decl)
            if pointer_match:
                type_name = pointer_match.group(1).strip()
                field_name = pointer_match.group(2)
                fields.append(StructField(
                    name=field_name,
                    type=type_name,
                    is_pointer=True
                ))
                continue
            
            # Regular field: type name
            regular_match = re.search(r'(\w+(?:\s+\w+)*)\s+(\w+)', decl)
            if regular_match:
                type_name = regular_match.group(1).strip()
                field_name = regular_match.group(2)
                fields.append(StructField(
                    name=field_name,
                    type=type_name
                ))
        
        return fields
    
    def resolve_type(self, type_name: str) -> str:
        """Resolve typedef aliases"""
        while type_name in self.typedefs:
            type_name = self.typedefs[type_name]
        return type_name
    
    def is_primitive(self, type_name: str) -> bool:
        """Check if a type is a primitive type"""
        resolved = self.resolve_type(type_name)
        return resolved in self.TYPE_MAP
    
    def get_type_info(self, type_name: str) -> Optional[Tuple[str, int]]:
        """Get type information (format, size)"""
        resolved = self.resolve_type(type_name)
        return self.TYPE_MAP.get(resolved)


class DynamicDecoder:
    """Dynamically generated decoder based on parsed structs"""
    
    def __init__(self, parser: StructParser, main_struct_name: str):
        self.parser = parser
        self.main_struct_name = main_struct_name
        self.structs = parser.structs
        
        # Dynamically create Python classes for each struct
        self._create_classes()
    
    def _create_classes(self):
        """Dynamically create Python classes for all structs"""
        for struct_name, struct_def in self.structs.items():
            # Create class attributes
            attrs = {'__annotations__': {}}
            
            for field in struct_def.fields:
                type_hint = self._get_type_hint(field)
                attrs['__annotations__'][field.name] = type_hint
            
            # Create the class
            cls = type(struct_name, (), attrs)
            
            # Add __init__ method
            def make_init(fields):
                def __init__(self, **kwargs):
                    for f in fields:
                        setattr(self, f.name, kwargs.get(f.name))
                return __init__
            
            cls.__init__ = make_init(struct_def.fields)
            
            # Add __repr__ method
            def __repr__(self):
                items = [f"{k}={v!r}" for k, v in self.__dict__.items()]
                return f"{self.__class__.__name__}({', '.join(items)})"
            cls.__repr__ = __repr__
            
            # Store in globals so it's accessible
            globals()[struct_name] = cls
    
    def _get_type_hint(self, field: StructField) -> type:
        """Get Python type hint for a field"""
        if field.is_array and field.type == 'char':
            return str
        if field.is_array:
            return list
        if field.is_pointer:
            return Any
        
        if self.parser.is_primitive(field.type):
            resolved = self.parser.resolve_type(field.type)
            if resolved in ['int', 'uint', 'unsigned int', 'short', 'unsigned short', 
                           'long', 'unsigned long', 'long long',
                           'int8_t', 'uint8_t', 'int16_t', 'uint16_t',
                           'int32_t', 'uint32_t', 'int64_t', 'uint64_t']:
                return int
            elif resolved in ['float', 'double']:
                return float
            elif resolved in ['bool', '_Bool']:
                return bool
            elif resolved == 'char':
                return str
        
        return Any
    
    def decode(self, b64_string: str) -> Any:
        """Decode base64 string to struct object"""
        binary_data = base64.b64decode(b64_string)
        
        if self.main_struct_name not in self.structs:
            raise ValueError(f"Main struct '{self.main_struct_name}' not found")
        
        obj, _ = self._decode_struct(self.main_struct_name, binary_data, 0)
        return obj
    
    def _decode_struct(self, struct_name: str, binary_data: bytes, 
                       offset: int) -> Tuple[Any, int]:
        """Decode a struct from binary data"""
        if struct_name not in self.structs:
            raise ValueError(f"Struct '{struct_name}' not found")
        
        struct_def = self.structs[struct_name]
        kwargs = {}
        
        for field in struct_def.fields:
            value, offset = self._decode_field(field, binary_data, offset)
            kwargs[field.name] = value
        
        # Create instance
        cls = globals()[struct_name]
        obj = cls(**kwargs)
        
        return obj, offset
    
    def _decode_field(self, field: StructField, binary_data: bytes, 
                     offset: int) -> Tuple[Any, int]:
        """Decode a single field from binary data"""
        
        # Handle char arrays (strings)
        if field.is_array and field.type == 'char':
            raw = struct.unpack_from(f'{field.array_size}s', binary_data, offset)[0]
            value = raw.decode('utf-8').rstrip('\x00')
            offset += field.array_size
            return value, offset
        
        # Handle other arrays
        if field.is_array:
            type_info = self.parser.get_type_info(field.type)
            if type_info:
                fmt, size = type_info
                value = list(struct.unpack_from(
                    f'{field.array_size}{fmt}', binary_data, offset
                ))
                offset += field.array_size * size
                return value, offset
            else:
                # Unknown array type, skip
                return None, offset
        
        # Handle pointers (skip for now - assume 8 bytes)
        if field.is_pointer:
            offset += 8
            return None, offset
        
        # Handle primitive types
        if self.parser.is_primitive(field.type):
            type_info = self.parser.get_type_info(field.type)
            if type_info:
                fmt, size = type_info
                value = struct.unpack_from(fmt, binary_data, offset)[0]
                offset += size
                return value, offset
        
        # Handle nested structs
        if field.type in self.structs:
            value, offset = self._decode_struct(field.type, binary_data, offset)
            return value, offset
        
        # Unknown type
        print(f"Warning: Unknown type '{field.type}' for field '{field.name}'")
        return None, offset


# ============================================================================
# Initialize the decoder
# ============================================================================

def create_decoder(header_files: List[str], main_struct: str):
    """
    Create a decoder from header files
    
    Args:
        header_files: List of header file paths to parse
        main_struct: Name of the main struct to decode
        
    Returns:
        DynamicDecoder instance or None if initialization fails
    """
    parser = StructParser()
    
    # Load headers from files
    for header_file in header_files:
        try:
            with open(header_file, 'r') as f:
                content = f.read()
                parser.parse(content)
                print(f"Loaded: {header_file}")
        except FileNotFoundError:
            print(f"Error: File not found: {header_file}")
            return None
        except Exception as e:
            print(f"Error reading {header_file}: {e}")
            return None
    
    print(f"\nParsed {len(parser.structs)} struct(s):")
    for name in parser.structs:
        print(f"  - {name}")
    
    if main_struct not in parser.structs:
        print(f"\nError: Main struct '{main_struct}' not found!")
        print(f"Available structs: {list(parser.structs.keys())}")
        return None
    
    decoder = DynamicDecoder(parser, main_struct)
    print(f"\nDecoder initialized for: {main_struct}")
    return decoder


def initialize_decoder():
    """Parse headers and create decoder using defaults"""
    if not HEADER_FILES or MAIN_STRUCT is None:
        print("Warning: Using initialize_decoder() with no default configuration")
        print("Use create_decoder(header_files, main_struct) instead")
        return None
    
    return create_decoder(HEADER_FILES, MAIN_STRUCT)


# Create the default decoder instance (if defaults are set)
decoder = initialize_decoder() if HEADER_FILES and MAIN_STRUCT else None


# ============================================================================
# Usage functions
# ============================================================================

def get_status(b64_string: str, decoder_instance=None):
    """
    Decode base64 telemetry string from instrument
    
    Args:
        b64_string: Base64 encoded string from instrument's getStatus command
        decoder_instance: Optional decoder instance (uses global decoder if None)
        
    Returns:
        Decoded telemetry object
    """
    dec = decoder_instance if decoder_instance is not None else decoder
    
    if dec is None:
        raise RuntimeError("Decoder not initialized. Use create_decoder() first.")
    
    return dec.decode(b64_string)


# ============================================================================
# Example usage
# ============================================================================

if __name__ == "__main__":
    print("="*60)
    print("Telemetry Decoder - Example Usage")
    print("="*60)
    
    # Example 1: Create decoder by passing parameters
    print("\n--- Method 1: Pass parameters to create_decoder() ---")
    
    my_decoder = create_decoder(
        header_files=["telemetry_t.h", "insthandle.h"],
        main_struct="telemetry_t"
    )
    
    if my_decoder:
        # Example b64 data (replace with actual instrument data)
        example_b64 = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA="
        
        try:
            # Decode using the created decoder
            status = get_status(example_b64, my_decoder)
            print(f"\nDecoded status: {status}")
            
            # Access fields
            print("\nAvailable fields:")
            for field in dir(status):
                if not field.startswith('_'):
                    print(f"  {field}: {getattr(status, field)}")
                    
        except Exception as e:
            print(f"Error decoding: {e}")
            print("Note: Example data may not match struct size")
    
    print("\n" + "="*60)
    print("To use in your main script:")
    print("="*60)
    print("""
from telemetry_decoder import create_decoder, get_status

# Create decoder for your instrument
decoder = create_decoder(
    header_files=["your_header.h"],
    main_struct="your_struct_t"
)

# Get status from instrument
status_b64 = instrument.getStatus()
status = get_status(status_b64, decoder)

# Access fields
print(f"User: {status.user}")
print(f"Temperature: {status.temp}")
    """)
