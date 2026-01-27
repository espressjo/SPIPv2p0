#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Auto-generated astro class from configuration file

This script parses cmd.cfg and dynamically generates methods for the astro class.
Supports inheritance - methods defined in parent classes will not be auto-generated.
"""
import socket
from time import sleep
import sys
from os.path import isfile
from datetime import datetime
from typing import Any, Dict, List, Optional, Union
from spip.telemetry import create_decoder, get_status

fmt = "%Y-%m-%dT%H:%M:%S.%fZ"

"""
Create decoder take the list of headers file where 
the telemetry header are define and the main structure
name. E.g.,
    #pragma pack(1,push)
    typedef struct {
    uint16_t a;
    test2 b;
    }test;
    #pragma pack(pop)
    
    #pragma pack(1,push)
    typedef struct {
    double a;
    double b;
    }test2;
    #pragma pack(pop)

test would be pass to create_decoder

Note: It is ver important to use the #pragma preprocessor
in the c/c++ headers otherwise the variable will be pack 
without buffer. Python will have a hard time figuring out 
how to interpret the data.

"""
telemetry_decoder = create_decoder(["/opt/spipv2p0/include/spip_telemetry_t.h","/usr/local/include/astro/insthandle.h"], "spip_telemetry_t")
"""
You have to set the cmd.conf full path so that the 
class knows which command to expect. Please see the section 
about the config file for more details.
"""
CMDCONFIGFILE = '/opt/spipv2p0/config/cmd.conf'


"""
You have to implement your own getstatus_t function to call the write 
status command given the cmd.conf file. Otherwise it is impossible to
know which file followe the right structure to decode the telemetry;

the command in getstatus_t HAS TO return 
    OK <length of encoded data> <b64 encoded message>
"""


def getstatus_t(ip,port):
    with spip(ip,port) as _astro:
        b64_status = _astro.b64spipstatus()#the right status function
        try:
            length,txt = b64_status.split(" ")
            length = int(length)
            #print("length: %d"%length)
            #print(txt)
            status = get_status(txt, telemetry_decoder)
            return status
        except:
            print("Fail to fetch telemetry")
        return None


class CommandConfig:
    """Represents a single command configuration"""
    def __init__(self, name: str, state: str):
        self.name = name
        self.state = state
        self.arguments: List[Dict[str, Any]] = []
        self.return_type = None  # None means only OK/NOK
        self.default_return_type = None  # Default return type for the command
        
    def add_argument(self, arg_name: str, optional: bool, arg_type: str, 
                     comment: str = "", arg_return_type: Optional[str] = None):
        """Add an argument to this command"""
        self.arguments.append({
            'name': arg_name,
            'optional': optional,
            'type': arg_type,
            'comment': comment,
            'is_flag': arg_name.startswith('-') and arg_type == 'string',
            'return_type': arg_return_type  # Specific return type for this arg
        })
        
    def set_return_type(self, return_type: str):
        """Set the expected return type (str, float, int)"""
        self.default_return_type = return_type
        
    def get_return_type(self, kwargs: Dict[str, Any]) -> Optional[str]:
        """Get the return type based on which arguments are used"""
        # Check if any argument with specific return type is being used
        for arg in self.arguments:
            if arg['return_type'] and arg['name'].lstrip('-') in kwargs:
                return arg['return_type']
        # Fall back to default return type
        return self.default_return_type


def parse_config_file(config_path: str) -> Dict[str, CommandConfig]:
    """Parse the cmd.cfg file and return dictionary of CommandConfig objects"""
    commands = {}
    current_cmd = None
    
    with open(config_path, 'r') as f:
        lines = f.readlines()
    
    for line in lines:
        line = line.strip()
        
        # Skip empty lines and separator lines
        if not line or line.startswith(':::'):
            continue
            
        # Parse CMD line
        if line.startswith('CMD'):
            parts = line.split()
            if len(parts) >= 3:
                cmd_name = parts[1]
                cmd_state = parts[2]
                current_cmd = CommandConfig(cmd_name, cmd_state)
                commands[cmd_name] = current_cmd
                
        # Parse ARG line
        elif line.startswith('ARG') and current_cmd:
            parts = line.split()
            if len(parts) >= 3:
                arg_name = parts[1]
                is_optional = parts[2] == 'optional'
                arg_type = parts[3] if len(parts) > 3 else 'string'
                
                # Extract comment if present
                comment = ''
                arg_return_type = None
                if '#' in line:
                    comment = line.split('#')[1].strip()
                
                # Check for return type annotation <type>
                if '<' in arg_type and '>' in arg_type:
                    # Extract return type from <int>, <float>, <str>
                    arg_return_type = arg_type[arg_type.index('<')+1:arg_type.index('>')]
                    arg_type = arg_type[:arg_type.index('<')].strip()
                    if not arg_type:
                        arg_type = 'string'
                
                current_cmd.add_argument(arg_name, is_optional, arg_type, 
                                        comment, arg_return_type)
                
        # Parse RETURN line
        elif line.startswith('RETURN') and current_cmd:
            parts = line.split()
            if len(parts) >= 2:
                return_type = parts[1].strip('<>')
                current_cmd.set_return_type(return_type)
    
    return commands


# ============================================================================
# Base class for custom method implementations
# ============================================================================

class AstroCustomMethods:
    """
    Base class containing custom implementations of specific commands.
    Inherit from this class to override auto-generated methods.
    """
    
    def b64status(self, parse_output=True):
        """
        Custom implementation for b64status command.
        This overrides the auto-generated version.
        
        Parameters
        ----------
        parse_output : bool, optional
            If True, parse and structure the output. Default is True.
            
        Returns
        -------
        dict or str or None : Parsed status dict if parse_output=True, 
                              raw string if False, None on failure
        """
        self._add2log("Getting b64 status (custom implementation)")
        
        if self.write("b64status") != 0:
            return None
        
        response = self.read().strip()
        
        if "NOK" in response or "nok" in response.lower():
            self._add2log(f"b64status failed: {response}")
            return None
        
        if not parse_output:
            return response
        
        # Parse the response into structured data
        status_dict = {}
        lines = response.split('\n')
        
        for line in lines:
            if ':' in line:
                key, value = line.split(':', 1)
                status_dict[key.strip()] = value.strip()
        
        return status_dict
    
    def telemetry(self, parse=True, **kwargs):
        """
        Custom implementation for telemetry command.
        
        Parameters
        ----------
        parse : bool, optional
            Parse telemetry data into structured format
        **kwargs : 
            all, b64, mux, asic, hxrg arguments
            
        Returns
        -------
        dict or str or None : Parsed telemetry if parse=True, raw otherwise
        """
        self._add2log("Getting telemetry (custom implementation)")
        
        cmd_parts = ['telemetry']
        
        # Build command
        if 'all' in kwargs and kwargs['all']:
            cmd_parts.append('-all')
        if 'b64' in kwargs and kwargs['b64']:
            cmd_parts.append('-b64')
        if 'mux' in kwargs:
            cmd_parts.extend(['mux', str(int(kwargs['mux']))])
        if 'asic' in kwargs and kwargs['asic']:
            cmd_parts.append('-asic')
        if 'hxrg' in kwargs and kwargs['hxrg']:
            cmd_parts.append('-hxrg')
        
        cmd = ' '.join(cmd_parts)
        
        if self.write(cmd) != 0:
            return None
        
        response = self.read().strip()
        
        if "NOK" in response:
            self._add2log(f"telemetry failed: {response}")
            return None
        
        if not parse:
            return response
        
        # Custom parsing logic for telemetry
        telemetry_dict = {}
        # Add your custom parsing here
        
        return telemetry_dict


# ============================================================================
# Main astro class - inherits custom methods
# ============================================================================

#class astro(AstroCustomMethods): example of inheritance
class spip():
    def __init__(self, host, port, buffer_size=4096, timeout=10.0, 
                 config_file=CMDCONFIGFILE):
        '''
        Description
            TCP socket-based connection to the HxRG-SERVER with auto-generated
            command methods from configuration file
        '''
        self.host = host
        self.port = port
        self.buffer_size = buffer_size
        self.timeout = timeout
        self.socket = None
        self.cmd_delay = 0.8  # sleep before each write call
        self.pread = 0
        self.ramp = 0
        self.integration_time = 0
        self.HxRG = 4
        self.nb_output = 32
        self.add2log = False
        self.add2logfile = ""
        
        # Parse configuration and dynamically generate methods
        if isfile(config_file):
            self._commands = parse_config_file(config_file)
            self._generate_methods()
        else:
            self._commands = {}
            print(f"Warning: Config file '{config_file}' not found. "
                  "No methods generated.")
    def getstatus_t(self):
        b64_status = self.getstatus()
        #check for length 1st
        length = ""
        txt = ""
        print(b64_status)
        try:
            check = b64_status.split(" ")
            print(check)
        except:
            print("[debug] failed to decode b64")
        print(txt)
        return 
    def _generate_methods(self):
        """Dynamically generate methods for each command in the config"""
        for cmd_name, cmd_config in self._commands.items():
            # Skip if method already exists (allows manual override)
            if hasattr(self.__class__, cmd_name):
                self._add2log(f"Skipping auto-generation for '{cmd_name}' "
                             "(manually defined)")
                continue
            
            # Create a method for this command
            method = self._create_command_method(cmd_config)
            # Bind it to the instance
            setattr(self, cmd_name, method.__get__(self, spip))
    
    def _create_command_method(self, cmd_config: CommandConfig):
        """Create a method for a specific command configuration"""
        def command_method(self, **kwargs):
            """
            Auto-generated method for command: {cmd_name}
            State: {state}
            
            Parameters
            ----------
            {params}
            
            Returns
            -------
            {return_info}
            """
            cmd_parts = [cmd_config.name]
            
            # Build command string from kwargs
            for arg in cmd_config.arguments:
                arg_name = arg['name'].lstrip('-')
                
                if arg_name in kwargs:
                    value = kwargs[arg_name]
                    
                    # Handle flag arguments (no value expected)
                    if arg['is_flag']:
                        if value:  # If True or non-empty
                            cmd_parts.append(arg['name'])
                    else:
                        # Handle arguments with values
                        cmd_parts.append(arg['name'])
                        
                        # Type conversion
                        if arg['type'] == 'int':
                            cmd_parts.append(str(int(value)))
                        elif arg['type'] == 'float' or arg['type'] == 'double':
                            cmd_parts.append(str(float(value)))
                        else:
                            cmd_parts.append(str(value))
                elif not arg['optional']:
                    self._add2log(f"Missing required argument: {arg['name']}")
                    return None
            
            cmd_string = ' '.join(cmd_parts)
            self._add2log(f"Executing: {cmd_string}")
            
            # Determine expected return type based on arguments used
            expected_return = cmd_config.get_return_type(kwargs)
            
            # Send command and get response
            if self.write(cmd_string) != 0:
                return None
            
            response = self.read().strip()
            
            # Parse response based on expected return type
            if expected_return is None:
                # Only expecting OK/NOK
                if "NOK" in response or "nok" in response.lower():
                    
                    self._add2log(f"Command failed: {response}")
                    return False
                elif "OK" in response or "ok" in response.lower():
                    return True
                else:
                    self._add2log(f"Command failed: {response}")
                    return False
            else:
                # Expecting OK <value> or NOK
                if "NOK" in response or "nok" in response.lower():
                    self._add2log(f"Command failed: {response}")
                    return None
                
                # Extract value from "OK <value>" format
                if "OK" in response or "ok" in response.lower():
                    # Remove OK/ok prefix
                    parts = response.split(maxsplit=1)
                    if len(parts) > 1:
                        value_str = parts[1]
                    else:
                        # OK with no value
                        self._add2log(f"Expected value but got only: {response}")
                        return None
                else:
                    # Assume entire response is the value (no OK prefix)
                    value_str = response
                
                # Convert to appropriate type
                try:
                    if expected_return == "int":
                        return int(value_str)
                    elif expected_return == "float":
                        return float(value_str)
                    else:  # str
                        return value_str
                except (ValueError, AttributeError) as e:
                    self._add2log(f"Failed to parse {expected_return} from: {value_str} ({e})")
                    return None
        
        # Generate docstring
        params_doc = []
        for arg in cmd_config.arguments:
            arg_display = arg['name'].lstrip('-')
            opt_str = "optional" if arg['optional'] else "required"
            comment_str = f" - {arg['comment']}" if arg['comment'] else ""
            return_str = f" -> returns {arg['return_type']}" if arg['return_type'] else ""
            params_doc.append(
                f"{arg_display} : {arg['type']} ({opt_str}){comment_str}{return_str}"
            )
        
        if cmd_config.default_return_type:
            return_info = (f"{cmd_config.default_return_type} or None\n"
                          f"            Returns the value if successful (OK <value>), "
                          f"None if failed (NOK)")
        else:
            return_info = "bool : True if successful (OK), False if failed (NOK)"
        
        command_method.__doc__ = command_method.__doc__.format(
            cmd_name=cmd_config.name,
            state=cmd_config.state,
            params='\n            '.join(params_doc) if params_doc else "None",
            return_info=return_info
        )
        
        return command_method
        
    def __enter__(self):
        '''Connect to the server when entering context'''
        self.connect()
        if "ECHO" not in self.read():
            raise Exception("Handshake with server")
        return self
        
    def __exit__(self, exc_type, exc_val, exc_tb):
        '''Disconnect from the server when exiting context'''
        self.disconnect()
        return False
        
    def connect(self):
        '''Establish TCP socket connection'''
        try:
            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.settimeout(self.timeout)
            self.socket.connect((self.host, self.port))
            self._add2log(f"Connected to {self.host}:{self.port}")
            return 0
        except Exception as e:
            self._add2log(f"Connection failed: {e}")
            return -1
            
    def disconnect(self):
        '''Close TCP socket connection'''
        if self.socket:
            try:
                self.socket.close()
                self._add2log("Disconnected from server")
            except Exception as e:
                self._add2log(f"Error during disconnect: {e}")
            finally:
                self.socket = None
                
    def write(self, cmd):
        '''Send command to server'''
        if not self.socket:
            self._add2log("Socket not connected")
            return -1
        try:
            sleep(self.cmd_delay)
            # Add newline if not present
            if not cmd.endswith('\n'):
                cmd += '\n'
            self.socket.sendall(cmd.encode('utf-8'))
            return 0
        except Exception as e:
            self._add2log(f"Write error: {e}")
            return -1
            
    def read(self, terminator='\n'):
        '''Read response from server'''
        if not self.socket:
            self._add2log("Socket not connected")
            return ""
        try:
            response = b""
            while True:
                chunk = self.socket.recv(self.buffer_size)
                if not chunk:
                    break
                response += chunk
                # Check if we have a complete response
                if terminator.encode('utf-8') in response:
                    break
            return response.decode('utf-8')
        except socket.timeout:
            self._add2log("Read timeout")
            return ""
        except Exception as e:
            self._add2log(f"Read error: {e}")
            return ""
            
    def write_get_OK(self, cmd):
        '''Send command and check for OK response'''
        if self.write(cmd) != 0:
            return -1
        response = self.read()
        if "OK" in response or "ok" in response.lower():
            return 0
        else:
            self._add2log(f"Command failed: {cmd}, Response: {response}")
            return -1
            
    def write_get_str(self, cmd, default=""):
        '''Send command and return string response'''
        if self.write(cmd) != 0:
            return default
        response = self.read()
        return response.strip() if response else default
        
    def _add2log(self, string):
        if not self.add2log:
            return
        with open(self.add2logfile, "a") as log:
            log.write("%s\t%s\n" % (datetime.now().strftime(fmt), string))
    
    def list_commands(self):
        """List all available auto-generated commands"""
        print("\nAvailable commands:")
        print("-" * 60)
        for cmd_name, cmd_config in self._commands.items():
            args_str = ", ".join([
                f"{arg['name']}({arg['type']})" 
                for arg in cmd_config.arguments[:3]
            ])
            if len(cmd_config.arguments) > 3:
                args_str += ", ..."
            print(f"{cmd_name:20s} [{cmd_config.state:10s}] {args_str}")
        print("-" * 60)
        print(f"Total: {len(self._commands)} commands\n")
    
    def command_info(self, command_name: str):
        """
        Get detailed information about a command's arguments
        
        Parameters
        ----------
        command_name : str
            Name of the command to inspect
            
        Returns
        -------
        dict or None : Command configuration details
        """
        if command_name not in self._commands:
            print(f"Command '{command_name}' not found")
            return None
        
        cmd = self._commands[command_name]
        
        print(f"\nCommand: {cmd.name}")
        print(f"State: {cmd.state}")
        print(f"Default Return Type: {cmd.default_return_type or 'OK/NOK (bool)'}")
        print("\nArguments:")
        print("-" * 80)
        
        for arg in cmd.arguments:
            arg_name = arg['name'].lstrip('-')
            opt = "optional" if arg['optional'] else "REQUIRED"
            flag = " (flag)" if arg['is_flag'] else ""
            ret_type = f" -> {arg['return_type']}" if arg['return_type'] else ""
            comment = f"  # {arg['comment']}" if arg['comment'] else ""
            
            print(f"  {arg['name']:20s} {arg['type']:10s} [{opt}]{flag}{ret_type}{comment}")
        
        print("-" * 80)
        
        # Return structured data
        return {
            'name': cmd.name,
            'state': cmd.state,
            'default_return_type': cmd.default_return_type,
            'arguments': cmd.arguments
        }
    
    def get_command_args(self, command_name: str) -> List[str]:
        """
        Get list of argument names for a command
        
        Parameters
        ----------
        command_name : str
            Name of the command
            
        Returns
        -------
        list : List of argument names (without leading '-')
        """
        if command_name not in self._commands:
            return []
        
        return [arg['name'].lstrip('-') for arg in self._commands[command_name].arguments]
    
    # Keep specialized methods that need custom logic
    def acqRunning(self):
        """Check if acquisition is currently running"""
        out = self.write_get_str("status -isAcqRunning")
        if 'T' in out:
            return True
        else:
            return False
    
    def b64status(self, parse_output=True):
        """
        Custom implementation for b64status command.
        This is manually defined and overrides auto-generation.
        
        Parameters
        ----------
        parse_output : bool, optional
            If True, parse and structure the output. Default is True.
            
        Returns
        -------
        dict or str : Parsed status dict if parse_output=True, raw string otherwise
        """
        self._add2log("Getting b64 status")
        
        if self.write("b64status") != 0:
            return None
        
        response = self.read().strip()
        
        if "NOK" in response or "nok" in response.lower():
            self._add2log(f"b64status failed: {response}")
            return None
        
        if not parse_output:
            return response
        
        # Parse the response into structured data
        # Example: customize this based on your actual b64status format
        status_dict = {}
        lines = response.split('\n')
        
        for line in lines:
            if ':' in line:
                key, value = line.split(':', 1)
                status_dict[key.strip()] = value.strip()
        
        return status_dict

    def start_acquisition(self, _telemetry=False):
        '''
        Trigger an acquisition with monitoring.
        This wraps the auto-generated 'start' command with progress tracking.
        
        Returns
        -------
        bool : True if successful, False if failed
        '''
        self._add2log("Starting acquisition")
        
        # Use the auto-generated start method
        if hasattr(self, 'start'):
            result = self.start(telemetry=_telemetry)
            if not result:
                self._add2log("Failed to start acquisition")
                return False
        else:
            # Fallback if start method not generated
            cmd = "start -telemetry" if _telemetry else "start"
            if self.write_get_OK(cmd) != 0:
                self._add2log("Failed to start acquisition")
                return False

        if self.HxRG == 2:
            x = 2048
        elif self.HxRG == 1:
            x = 1024
        else:
            x = 4096
        self.integration_time = 1e-5 * x**2 / self.nb_output
        sleep(5.5)  # make sure we don't poll too fast
        total = self.integration_time * self.pread * self.ramp

        while(self.acqRunning()):
            sys.stdout.write(f"\r{' ' * 30}\rCountdown: {total:.1f} seconds")
            sys.stdout.flush()
            total -= 1
            sleep(1)
        self._add2log("acquisition complete")
        return True
    
    def generic_io(self, cmd):
        """Send arbitrary command and return response"""
        self.write(cmd)
        out = self.read()
        return out.strip()


if '__main__' == __name__:
    # Test the auto-generation
    print("Astro3 - Auto-generated Command Interface with Inheritance")
    print("=" * 60)
    
    # Create instance (will auto-generate methods from cmd.cfg)
    astro_client = spip("localhost", 9555, config_file='cmd.cfg')
    
    # List all generated commands
    astro_client.list_commands()
    
    print("\nInheritance Example:")
    print("-" * 60)
    print("Methods defined in AstroCustomMethods parent class:")
    print("  - b64status (custom parsing)")
    print("  - telemetry (custom parsing)")
    print("\nThese will NOT be auto-generated. All other commands")
    print("from cmd.cfg will be auto-generated.")
    print("-" * 60)
    
    print("\nReturn Type Examples:")
    print("-" * 60)
    print("1. No RETURN specified (default OK/NOK):")
    print("   result = client.init()  # Returns: True or False")
    print()
    print("2. RETURN <str> specified:")
    print("   value = client.asicio(addr='0x10', read=True)")
    print("   # Returns: 'value_string' or None")
    print()
    print("3. Custom inherited method:")
    print("   status = client.b64status(parse_output=True)")
    print("   # Returns: dict or None (from AstroCustomMethods)")
    print("-" * 60)
    
    print("\nInspecting commands:")
    print("-" * 60)
    
    # Show detailed info for a specific command
    if 'asicio' in astro_client._commands:
        astro_client.command_info('asicio')
    
    # Check which methods are custom vs auto-generated
    print("\nMethod source check:")
    print(f"  b64status: {'CUSTOM (from parent)' if hasattr(AstroCustomMethods, 'b64status') else 'auto-generated'}")
    print(f"  telemetry: {'CUSTOM (from parent)' if hasattr(AstroCustomMethods, 'telemetry') else 'auto-generated'}")
    print(f"  init: {'CUSTOM' if 'init' in dir(AstroCustomMethods) else 'auto-generated'}")
    
    print("\nExample usage:")
    print("  # Use custom inherited methods:")
    print("  status = client.b64status(parse_output=True)")
    print("  telem = client.telemetry(all=True, parse=True)")
    print()
    print("  # Use auto-generated methods:")
    print("  if client.init(): print('Success')")
    print("  value = client.asicio(addr='0x10', read=True)")
    
    print("\n" + "=" * 60)
    print("To add more custom methods, define them in AstroCustomMethods")
    print("=" * 60)
