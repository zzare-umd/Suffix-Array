import matplotlib.pyplot as plt
import numpy as np
import pdb


# Initialize plot
fig = plt.figure()
fig.set_size_inches(5,5)
ax = fig.add_subplot(111)


# File Extensions and Sizes
fnames = ["10K","20K","2M","_4.4","48M"]
labels = ["11KB","20KB","1.6MB","4.6MB","49MB"]

# Loop over each file name extension
for dd in range(len(fnames)):

	kval,time = np.loadtxt("elapsed_time%s.txt" % fnames[dd],unpack=True)

	# plot the data
	ax.plot(kval,time,marker="o",mec="black",mew=0.5,ls="--",label=labels[dd])


# Define Legend
ax.legend(frameon=False,ncol=3,numpoints=1,loc="upper right")

# Define Labels
ax.set_title(r"build\_sa() time")
ax.set_xlabel(r"$k$ (prefix size)")
ax.set_ylabel(r"elapsed time (sec)")


# Define Axis Limits and Scaling
ax.set_yscale("log")
ax.set_ylim(7e-4,3e2)


# Save Plot
plt.savefig("elapsed_time.png",format="png",dpi=300,bbox_inches="tight")

