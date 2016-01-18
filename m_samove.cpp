 /*
 * m_samove -- InspIRCd Module
 *
 *   Copyright (C) 2016 Michael Norris
 * 
 * This file contains a third party module for InspIRCd.  You can
 * redistribute it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, version 2.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "inspircd.h"

/* $ModDesc: Provides command SAMOVE which allows opers to part a user from a channel and rejoin them in a different channel */

/** Handle /SAMOVE
 */
class CommandSamove : public Command
{
 public:
	CommandSamove(Module* Creator) : Command(Creator,"SAMOVE", 3, 4)
	{
		allow_empty_last_param = false;
		flags_needed = 'o'; Penalty = 0; syntax = "<nick> <from-channel> <to-channel> [reason]";
	}

	CmdResult Handle (const std::vector<std::string>& parameters, User *user)
	{
		
		User* target = ServerInstance->FindNick(parameters[0]);
		Channel* part_channel = ServerInstance->FindChan(parameters[1]);
		std::string reason = user->nick.c_str();
		std::string part_msg = "Switching channels";

		if ((target) && (target->registered == REG_ALL) && (part_channel))
		{
			if (parameters.size() > 3)
				reason = parameters[3];

			if (ServerInstance->ULine(target->server))
			{
				user->WriteNumeric(ERR_NOPRIVILEGES, "%s :Cannot use an SA command on a u-lined client", user->nick.c_str());
				return CMD_FAILURE;
			}
			
			if(!ServerInstance->IsChannel(parameters[2].c_str(), ServerInstance->Config->Limits.ChanMax))
			{
				user->WriteServ("NOTICE " + user->nick + " :*** Invalid characters in channel name or name too long");
				return CMD_FAILURE;
			}

			if(strcmp(parameters[1].c_str(), parameters[2].c_str()) == 0){
				user->WriteServ("NOTICE " + user->nick + " :*** Parting channel cannot be the same as the joining channel");
				return CMD_FAILURE;
			}

			part_channel->PartUser(target, part_msg);
			Channel* a = ServerInstance->FindChan(parameters[1]);	
			if(a)
			{
				if(a->HasUser(target)){
					user->WriteServ("NOTICE %s :*** Unable to make %s part %s", user->nick.c_str(), target->nick.c_str(), parameters[1].c_str());
					return CMD_FAILURE;
				}
			}
			
			Channel::JoinUser(target, parameters[2].c_str(), true, "", false, ServerInstance->Time());
			Channel* b = ServerInstance->FindChan(parameters[2]);
			if (b)
			{
				if (b->HasUser(target))
				{
					ServerInstance->SNO->WriteGlobalSno('a', user->nick + " used SAMOVE to move " + target->nick+ " from " + parameters[1] + " to " + parameters[2]);
					user->WriteTo(target, "NOTICE %s : You have been moved from %s to %s (%s)", target->nick.c_str(), parameters[1].c_str(), parameters[2].c_str(), reason.c_str());
					return CMD_SUCCESS;
				}
				else
				{
					user->WriteServ("NOTICE " +user->nick+ " :*** Could not join " + target->nick + " to " +parameters[2] + " (User is probably banned, or blocking modes)");
					return CMD_FAILURE;
				}
			}
			else
			{
				user->WriteServ("NOTICE " + user->nick + " :*** Could not join " + target->nick + " to " + parameters[2]);
				return CMD_FAILURE;
			}
			return CMD_SUCCESS;
		}
		
		else
		{
			user->WriteServ("NOTICE " + user->nick + " :*** Invalid nickname or channel");
		}
		return CMD_FAILURE;
	}
};

class ModuleSamove : public Module
{
	CommandSamove cmd;
	
 public:
	ModuleSamove() 
		: cmd(this)
	{
	}
	
	void init()
	{
		ServerInstance->Modules->AddService(cmd);
	}

	Version GetVersion()
	{
		return Version("Provides command SAMOVE which allows opers to part a user from a channel and rejoin them in a different channel", VF_NONE);
	}
};

MODULE_INIT(ModuleSamove)
