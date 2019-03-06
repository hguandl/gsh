# THE GUAN SHELL (GSH)

## Introducion

Gsh is a toy software for operating system course, which is a good practice of comprehending system calls including `fork()` and `exec()`. It is not stable, but for educational and experimental use. Please use under your own risk.

Current version: 1.0.1 | [ChangeLog](ChangeLog)

Travis CI:

## Features
- Bash-like prompt
- Execute system commands
- History and auto-completion

## Installation
Gsh supports Linux and macOS with [readline](https://tiswww.case.edu/php/chet/readline/rltop.html) library.

### Get library
#### macOS
macOS contains [editline](http://troglobit.com/editline.html) as an alias of readline, which is mostly compatible. Thus there is no need to install the library. But still note that they would behave differently on some rare circumstances. If you want to use GNU readline, please follow these steps:
```bash
$ brew install readline
$ export LD_FLAGS="-L/usr/local/opt/readline/lib"
```
#### Debian / Ubunru
```bash
$ sudo apt-get install libreadline-dev
```
#### RedHat / CentOS
```bash
$ sudo yum install readline-devel
```

### Build from source
```bash
$ autoreconf --install --force
$ ./configure
$ make && make install
```

## Get Started
```bash
$ gsh
```
And now you are in G-shell. The enviroment variables are inherited. After some try, you can exit with Ctrl+D (^D) or `exit`.

## Further Usage
### Add gsh to acceptable shells
Add the absolute path of gsh in `/etc/shells`.
For example, add a new line:
```bash
# /etc/shells:

/bin/bash
/bin/sh
/usr/local/bin/gsh
```

### Change default shell to gsh
Use `chsh -s` to change your default shell.
```bash
$ chsh -s /usr/local/bin/gsh
```

### Enjoy
Open a new terminal and login, now you are using gsh.

## Known Issues
- Only `cd` and `exit` are implemented. Other build-in commands would be added in future versions.
- Multi-line commands have not been supported so far.
- History is in memory, not stored in files.
- Enviroment variables has not been implemented.

## TODO
- [ ] enviroment variables
- [ ] .gsh_history
- [ ] `;` (line separator)
- [ ] `|` (pipe)
- [ ] `if`
- [ ] `for`

## License
```
Copyright: 2017-2019, hguandl <me@hguandl.com>

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
```
