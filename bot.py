import discord
import os
from constants import *
from niki_process import *

startNikiCMD()

class NikiBot(discord.Client):
	async def on_ready(self):
		await tree.sync(guild=discord.Object(GUILD_ID))
		await tree.sync()
		print(f'Logged on as {self.user}!')

intents = discord.Intents.default()
intents.message_content = True
intents.dm_messages = True

client = NikiBot(intents=intents)
tree = discord.app_commands.CommandTree(client)

@tree.command(
    name="nikiscript",
    description="Interprets text as NikiScript",
	guild=discord.Object(GUILD_ID)
)
async def script(interaction: discord.Interaction, script: str):
	if not isOwner(interaction) and interaction.channel_id != CHANNEL_ID:
		await interaction.response.send_message(f"Can not run nikiscript here. Try on <#{CHANNEL_ID}> instead.", ephemeral=True)
		return

	await interaction.response.defer()
	await runNikiScript(interaction, script);

@tree.command(
	name='nikiscript2',
	description="Interprets text as NikiScript"
)
@discord.app_commands.check(isOwner)
@discord.app_commands.allowed_contexts(guilds=True, dms=True, private_channels=True)
async def script(interaction: discord.Interaction, script: str, hidden: bool=True):
	await interaction.response.defer(ephemeral=hidden)
	await runNikiScript(interaction, script)

client.run(os.getenv('DISCORD_BOT_TOKEN'))