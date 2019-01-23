
#!/bin/bash
# Copyright (C) tkornuta, IBM Corporation 2015-2019
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Stop the script on first error.
set -e

# Setting a return status for a function
download_latest_release () {
    user=$1
    repo=$2    
    echo "Downloading latest release from: ${user}/${repo}"

    # Find the archive.
    version=$( curl --silent "https://api.github.com/repos/${user}/${repo}/releases/latest" | grep '"tag_name":' | sed -E 's/.*"([^"]+)".*/\1/' )
    echo "Found version: ${version}"

    # Download the archive.
    echo "Downloading. Please wait..."
    curl -sOL "https://github.com/${user}/${repo}/archive/${version}.tar.gz"

    # Unpack the archive.
    echo "Unpacking the archive"
    #mkdir ${repo}
    tar xzvf ${version}.tar.gz
    # Rename directory.
    mv ${repo}-${version} ${repo}

    # Cleanup.
    rm ${version}.tar.gz
    echo "Done"
    return 0
}

