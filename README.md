#m_samove - InspIRCd Module

Description
-

m_samove is a third party InspIRCd module that allows opers to move a user from one channel into another channel.

Installation
-

1. Download the file m_samove.cpp and place it in the '/src/modules/' directory of your InspIRCd installation.

2. Add the following code to your 'modules.conf' file

        #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#
        # SAMOVE module: Adds the /SAMOVE command which forcibly parts a user
        # from one channel and joins them to another channel.
        # This module is oper-only.
        # To use, SAMOVE must be in one of your oper class blocks.
        <module name="m_samove.so">

3. Modify your 'opers.conf' file to include 'SAMOVE' as a command under the SACommands oper class. e.g:

        <class name="SACommands" commands="SAJOIN SAPART SANICK SAQUIT SATOPIC SAKICK SAMODE OJOIN SAMOVE">

4. Run the following command from the root InspIRCd directory:


        make && make install
    
    
5. Restart the InspIRCd server or load the module from the server:

        /loadmodule m_samove.so

Usage
-

You must be identified as an oper with SAMOVE command privileges to use this. The following command would part \<user> from \<channel1> and join them into \<channel2>. \<user> will also receive a notice upon being moved with the optional argument [reason].

    /SAMOVE <nick> <channel1> <channel2> [reason]
