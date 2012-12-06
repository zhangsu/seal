require 'rake/extensiontask'

desc 'Run the default task'
task :default => :all

desc 'Run all tasks'
task :all => [:compile, :yard]

Rake::ExtensionTask.new('seal')

desc 'Run RSpec using Win32API binding'
task :win32spec do
  sh 'rspec -I win32api -r ./win32api/rspec_config.rb -f h > win32spec.html'
end

rule /^demo:/ do |r|
  Rake::Task[:compile].invoke
  sh 'ruby -I lib -X demo %s.rb' % r.name[/(?<=:).+/]
end
