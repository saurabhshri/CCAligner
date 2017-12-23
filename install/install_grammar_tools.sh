echo "You must be root to install grammar tool dependencies. Alternatively, you may install them manually." 2>&1
echo "The following dependencies must be met for generating grammar and language models : "
echo "CMUCLMTK : https://sourceforge.net/projects/cmusphinx/files/cmuclmtk/0.7/"
echo "g2p-seq2seq : https://github.com/cmusphinx/g2p-seq2seq"

read -p "Press Enter to continue, ctrl + c to quit."

echo "Installing CMUCLMTK - For Creating Language Models on the fly,"
echo "you may alternatively use the web service to generate the same."
mkdir dependencies
cd dependencies/
echo "Downloading CMUCLMTK : "
wget https://github.com/saurabhshri/mirror/raw/master/cmuclmtk-0.7.tar.gz
echo "Uncompressing toolkits : "
tar xvpzf cmuclmtk-0.7.tar.gz
cd cmuclmtk-0.7
./configure
make
sudo make install
sudo ldconfig
echo "Leaving cmuclmtk-0.7"
cd ../
echo "Installing and Downloading Tensorflow : "
sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-1.0.0-cp27-none-linux_x86_64.whl
echo "Downloading g2p-seq2seq - For Creating dictionaries"
wget https://github.com/saurabhshri/mirror/raw/master/g2p-seq2seq-master.zip
unzip g2p-seq2seq-master.zip
echo "Installing g2p-seq2seq"
cd g2p-seq2seq-master/
sudo python setup.py install
cd ../
cp -rf ../src/lib_ext/model .
cp -rf ../src/lib_ext/g2p-seq2seq-cmudict .
