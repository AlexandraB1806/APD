package task1;

import java.util.ArrayList;
import java.util.concurrent.ExecutorService;

public class PathFinder implements Runnable {
	private ArrayList<Integer> currentPath;
	private ExecutorService tpe;
	private final int destination;

	public PathFinder(ArrayList<Integer> current, ExecutorService tpe, int destination) {
		this.currentPath = current;
		this.tpe = tpe;
		this.destination = destination;
	}

	@Override
	public void run() {
		if (currentPath.get(currentPath.size() - 1) == destination) {
			System.out.println(currentPath);
			tpe.shutdown();
		}

		int lastNodeInPath = currentPath.get(currentPath.size() - 1);

		for (int [] edge : Main.graph) {
			if (edge[0] == lastNodeInPath && !currentPath.contains(edge[1])) {
				ArrayList<Integer> newPath = (ArrayList<Integer>) currentPath.clone();
				newPath.add(edge[1]);

				tpe.submit(new PathFinder(newPath, tpe, destination));
			}
		}
	}
}
