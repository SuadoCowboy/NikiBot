import discord

async def help_command(interaction: discord.Interaction, _command: str, _args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	out = '```lua\n'
	for command in commands:
		out += command+'\n'
	out += '```'

	await interaction.channel.send(content=out)

async def say_command(interaction: discord.Interaction, command: str, args: list[str], isOwner: bool):
	if not interaction.app_permissions.send_messages and not isOwner:
		return

	out = ''
	for arg in args:
		out += arg+' '

	await interaction.channel.send(content=out.replace('@everyone', 'no u').replace('@here', 'nope'))

commands = {'help': help_command, 'say': say_command}
