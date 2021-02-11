[ -f .shard.log ] && rm .shard.log	# Remove previous logs
termite & 				# Start a terminal
sh $HOME/.fehbg				# Set background 
xset r rate 300 50			# Set keyboard rate (not required)
exec shard >> .shard.log		# Start Shard
