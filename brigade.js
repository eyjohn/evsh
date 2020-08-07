const { events, Job } = require("brigadier");

function notifyGithub(status, description, e, p) {
  var j = new Job("gh-" + status, "technosophos/github-notify:latest");
  j.env = {
    GH_REPO: p.repo.name,
    GH_STATE: status,
    GH_DESCRIPTION: description,
    GH_CONTEXT: "brigade",
    GH_TOKEN: p.repo.token,
    GH_COMMIT: e.revision.commit
  };
  return j.run();
}

function createBuildAndTestJob() {
  var job = new Job("buildAndTest");

  job.image = "rikorose/gcc-cmake:gcc-9";

  job.tasks = [
    "apt update && apt-get install $(grep -vE \"^\s*#\" /src/dpkg-build-depends.lst | tr \"\n\" \" \")",
    "cd /build",
    "cmake /src",
    "make",
    "make test"
  ];

  job.storage.enabled = true;
  job.storage.path = '/build';

  job.streamLogs = true;
  return job;
}

async function runBuildAndTest(event, project) {
  const job = createBuildAndTestJob();
  notifyGithub("pending", "Starting build and test", event, project);
  try {
    await buildJob.run();
    notifyGithub("success", "Successfully finished build and test", event, project);
  } catch (err) {
    notifyGithub("failure", "Failed build and test, error: " + err, event, project);
  }
}

events.on("pull_request", runBuildAndTest);
