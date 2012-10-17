require 'rake/extensiontask'

desc 'Run the default task'
task :default => :all

desc 'Run all tasks'
task :all => [:compile, :yard]

Rake::ExtensionTask.new('seal')

rule /^demo:/ do |r|
  Rake::Task[:compile].invoke
  sh 'ruby -I lib -X demo %s.rb' % r.name[/(?<=:).+/]
end
