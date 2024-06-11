import json

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

# Read data (../data/Cannon_best_angle_data.json)
data = json.load(open('data/Cannon_best_angle_data.json'))

# Create a DataFrame
df = pd.json_normalize(data['data'])
print(df.info())

df['v'] = np.sqrt(df['current_state.v_x'] ** 2 + df['current_state.v_y'] ** 2)

# Plot the data
figure, axes = plt.subplots(3, 2, figsize=(18, 9))

palette = sns.color_palette("mako_r", 6)

fig = sns.lineplot(data=df, x='current_state.x', y='current_state.y', ax=axes[0, 0])
fig.set_title(f'Cannonball trajectory (angle = {data["angle"]}°)')
fig.set(xlabel='Distance (m)')
fig.set(ylabel='Height (m)')
fig.set_aspect('equal')
fig.grid(True)

fig = sns.lineplot(data=df, x='current_state.x', y='v', ax=axes[0, 1])
fig.set_title(f'Cannonball speed')
fig.set(xlabel='Distance (m)')
fig.set(ylabel='Speed (m/s)')
fig.grid(True)

fig = sns.lineplot(data=df, x='current_state.x', y='current_state.v_x', ax=axes[1, 0])
fig.set_title(f'Cannonball speed')
fig.set(xlabel='Distance (m)')
fig.set(ylabel='Speed x (m/s)')
fig.grid(True)

fig = sns.lineplot(data=df, x='current_state.x', y='current_state.v_y', ax=axes[1, 1])
fig.set_title(f'Cannonball speed')
fig.set(xlabel='Distance (m)')
fig.set(ylabel='Speed y (m/s)')
fig.grid(True)

fig = sns.lineplot(data=df, x='current_state.x', y='rhs.a_x', ax=axes[2, 0])
fig.set_title(f'Cannonball acceleration')
fig.set(xlabel='Distance (m)')
fig.set(ylabel='Acceleration x (m/s²)')
fig.grid(True)

fig = sns.lineplot(data=df, x='current_state.x', y='rhs.a_y', ax=axes[2, 1])
fig.set_title(f'Cannonball acceleration')
fig.set(xlabel='Distance (m)')
fig.set(ylabel='Acceleration y (m/s²)')
fig.grid(True)

plt.tight_layout()
plt.show()

# Save the plot
figure.savefig('data/Cannon_best_angle_plot.png')
